#include <Arduino.h>
#include "app_main.h"
#include "app_detector.h"
#include "app_camera.h"
#include "app_common.h"
#include "app_ipc.h"
#include "auxiliary/app_ui.h"
#include "Firebase_ESP_Client.h"
#include <esp_task_wdt.h>
#include <addons/TokenHelper.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

String devicename = "I2DS SENTINEL";
AppCamera appCam((uint8_t)IR_LED);
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
/* Pass context to app detector object */
detector_ctx_t det_ctx;
AppDetector appDet(&det_ctx);
TaskHandle_t dt;
QueueHandle_t ipcQueue;
TimerHandle_t ipcWarningRateLimiter;
TimerHandle_t firebaseUpdateTimer;
TimerHandle_t detectorOnHoldTimer;
QueueHandle_t firebaseSendPayloadQueue;
QueueHandle_t firebasePostQueue;
bool updateFirebase = false;
bool holdDetector = false;
bool requireCameraInitToGreyscale = true;
uint8_t det_pos_window = 0;
Point Sentinel("Sentinel");

/*! detectorOnHoldTimerCallback()
   @brief callback for Firebase regular updates


   @note sets flag at interval FIREBASE_UPDATE_INTERVAL_MS, aborted if request in progress

   @param firebaseUpdateTimer FreeRTOS timer handle
*/
void detectorOnHoldTimerCallback(TimerHandle_t detectorOnHoldTimer)
{
    holdDetector = false;
    APP_LOG_INFO("---------------DECTECTOR FREED----------------");
    requireCameraInitToGreyscale = true;
}

/*! firebaseUpdateTimerCallback()
   @brief callback for Firebase regular updates


   @note sets flag at interval FIREBASE_UPDATE_INTERVAL_MS, aborted if request in progress

   @param firebaseUpdateTimer FreeRTOS timer handle
*/
void firebaseUpdateTimerCallback(TimerHandle_t firebaseUpdateTimer)
{
    updateFirebase = true;
    APP_LOG_INFO("Firebase update requested");
}

/*! fcsUploadCallback()
   @brief callback for Firebase storage uploads
   @note
   @param info FCS_UploadStatusInfo
*/
void fcsUploadCallback(FCS_UploadStatusInfo info)
{
   // SPIFFS.remove(info.localFileName.c_str());
}

/*! appMainTask()
   @brief main task for sentinel. intializes camera, detector, and firebase.
   if nothing else to do, waits until detector ctx passes semaphore, then takes image and passes image pointer and semaphore back to detector.
   else rx ipc_warn_t from ipcQueue, switches camera mode to jpeg and commands camera to take image. image is passed back to this task through
   img_info_t from firebaseSendPayloadQueue, sent to firebase STORAGE, deleted from SPIFFS, then passed to firebasePostQueue to be appended to firebase RTDB.
   and for every FIREBASE_UPDATE_INTERVAL_MS updates self status info in firebase RTDB.
   @note

   @param void
*/
void appMainTask(void *pvParameters)
{

    esp_task_wdt_init(15, true);
    appDet.startDetectorTask(dt, 1, 1); // Start detector task with task handle dt on core 1

    ipcWarningRateLimiter = xTimerCreate("ipcWarningRateLimiter", pdMS_TO_TICKS(5500), pdFALSE, NULL, NULL);
    firebaseUpdateTimer = xTimerCreate("firebaseUpdateTimer", pdMS_TO_TICKS(5000), pdTRUE, NULL, firebaseUpdateTimerCallback);
    detectorOnHoldTimer = xTimerCreate("detectorOnHoldTimer", pdMS_TO_TICKS(30000), pdFALSE, NULL, detectorOnHoldTimerCallback);
    firebaseSendPayloadQueue = xQueueCreate(2, sizeof(img_info_t));
    firebasePostQueue = xQueueCreate(5, sizeof(img_info_t));
    ipcQueue = xQueueCreate(2, sizeof(ipc_warn_t));
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname(devicename.c_str()); // define hostname
    while (WiFi.status() != WL_CONNECTED)
    {
        APP_LOG_INFO("[INIT] ERROR CONNECTING TO WIFI");
        WiFi.disconnect();
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        vTaskDelay(3500);
    }

    Firebase.RTDB.setMaxRetry(&fbdo, 3);
    Firebase.RTDB.setMaxErrorQueue(&fbdo, 5);
    // Firebase.RTDB.allowMultipleRequests(&fbdo);
    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(FIREBASE_DATA_OBJECT_PAYLOAD_SIZE_BYTES);
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    config.fcs.upload_buffer_size = 8192;
    config.tcp_data_sending_retry = 15;
    config.timeout.wifiReconnect = 10 * 1000;
    config.timeout.socketConnection = 15 * 1000;
    config.timeout.serverResponse = 15 * 1000;
    config.timeout.rtdbKeepAlive = 30 * 1000;
    config.timeout.rtdbStreamReconnect = 1 * 1000;
    config.timeout.rtdbStreamError = 3 * 1000;
    config.token_status_callback = tokenStatusCallback;
    Firebase.signUp(&config, &auth, "", "");
    Firebase.begin(&config, &auth);

    if (appCam.init() != ESP_OK)
    {
        APP_LOG_INFO("[INIT] ERROR INITIALIZING CAMERA");
        vTaskDelay(1000);
    }
    timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

    client.setHTTPOptions(HTTPOptions().httpReadTimeout(200));
    client.setHTTPOptions(HTTPOptions().connectionReuse(true));
    // Check server connection
    client.validateConnection();
    xTimerStart(firebaseUpdateTimer, 0);

    Firebase.RTDB.deleteNode(&fbdo, "/sentinel");

    while (1)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            APP_LOG_INFO("[INIT] ERROR CONNECTING TO WIFI");
            WiFi.disconnect();
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
            vTaskDelay(3500);
        }

        if (updateFirebase && uxQueueMessagesWaiting(firebaseSendPayloadQueue) == 0)
        {
            APP_LOG_INFO("UPDATING FIREBASE");
            FirebaseJson selfInfoJson;
            String device = "/sentinel";
            selfInfoJson.set("timestamp/.sv", "timestamp");
            selfInfoJson.add("wifi", (String)WIFI_SSID);
            selfInfoJson.add("alert", 0);
            while (uxQueueMessagesWaiting(firebasePostQueue) > 0)
            {
                selfInfoJson.add("alert", 1);
                img_info_t info;
                if (xQueueReceive(firebasePostQueue, &info, 1) == pdPASS)
                {
                    APP_LOG_WARN("APPENDING NEW IMAGE DATA TO FIREBASE");
                    FirebaseJson imgInfoJson;
                    imgInfoJson.set("Source Node", info.ipc_info.src_id);
                    imgInfoJson.set("RSSI", info.ipc_info.rssi);
                    imgInfoJson.set("LQI", info.ipc_info.lqi);
                    imgInfoJson.set("Timestamp/.sv", "timestamp");
                    selfInfoJson.set((String)info.index, imgInfoJson);
                }
            }

            if (!Firebase.RTDB.updateNode(&fbdo, device, &selfInfoJson))
            {
                APP_LOG_ERR("FAILED TO UPDATE FIREBASE");
                APP_LOG_ERR(fbdo.errorReason().c_str());
            }
            Sentinel.clearFields();
            Sentinel.clearTags();
            Sentinel.addTag("UID", "N/A");
            Sentinel.addField("DetectionResult", (det_pos_window > 0) ? true : false);
            if (!client.writePoint(Sentinel))
            {
                APP_LOG_ERR("INFLUXDB UPLOAD ERROR");
            }

            updateFirebase = false;
        }
        /* Detector indicates it is ready by giving the semaphore. Do not take if holdDetector is set */
        if (!holdDetector && (xSemaphoreTake(det_ctx.send, 1) == pdTRUE))
        {
            /* Log results of detection from previous frame */
            if (det_ctx.result.success)
            {
                if (det_ctx.result.detected)
                    det_pos_window = 5;
                else
                {
                    if (det_pos_window > 0)
                        det_pos_window--;
                }
                APP_LOG_INFO("Person detected score: " + (String)det_ctx.result.posConfidence);
                APP_LOG_INFO("No person detected score: " + (String)det_ctx.result.negConfidence);
            }
            else
                APP_LOG_INFO("ERROR DETECTING");

            /* If detector was previously on hold from jpeg mode, reinit greyscale mode */
            if (requireCameraInitToGreyscale)
            {
                requireCameraInitToGreyscale = false;
                appCam.switchToGreyscale();
                APP_LOG_INFO("---------------SW GREYSCALE----------------");
            }

            /* Capture new image and pass returned pointer to detector */
            if (uxQueueSpacesAvailable(det_ctx.frame) == 0)
                xQueueReset(det_ctx.frame);
            uint8_t *frameBuf = appCam.captureGreyscale();
            if (xQueueSend(det_ctx.frame, (void *)&frameBuf, 0) != 0)
            {
            }
            appCam.returnFrameBuffer();

            /* Indiate to uiTask the results of detection (128 ? 0) */
            if (uxQueueSpacesAvailable(uiQueue) == 0)
                xQueueReset(uiQueue);
            uint8_t res = det_ctx.result.detected ? 128 : 0;
            if (xQueueSend(uiQueue, (void *)&res, 0) != 0)
            {
            }
        }
        if (!Firebase.ready())
        {
            Firebase.signUp(&config, &auth, "", "");
            Firebase.begin(&config, &auth);
        }

        /* Handle IPC warning. Payload information is sent to Firebase RTDB /SENTINEL/last and image is sent to database */
        if (uxQueueMessagesWaiting(ipcQueue) > 0)
        {
            ipc_warn_t warn;
            if (xQueueReceive(ipcQueue, &warn, 1) == pdPASS)
            {
                String filename = appCam.switchToJPEG(warn, holdDetector ? false : true);
                if (!holdDetector)
                    APP_LOG_INFO("---------------SW JPEG----------------");
                holdDetector = true;
                APP_LOG_INFO("---------------DECTECTOR BLOCKED----------------");
                xTimerReset(detectorOnHoldTimer, 0);
            }
        }

        if (uxQueueMessagesWaiting(firebaseSendPayloadQueue) > 0)
        {
            img_info_t info;
            if (xQueueReceive(firebaseSendPayloadQueue, &info, 1) == pdPASS)
            {
                if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */, info.path /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, info.path /* path of remote file stored in the bucket */, "image/jpeg" /* mime type */))
                {
                    // todo spiffs remove file
                    info.send_done = true;
                    if (xQueueSendToFront(firebasePostQueue, (void *)&info, 0) != 0)
                    {
                    }
                }
                else
                {
                    APP_LOG_ERR("FIREBASE STORAGE ERROR");
                    APP_LOG_ERR(fbdo.errorReason());
                    info.send_err = true;
                    xQueueSendToBack(firebaseSendPayloadQueue, (void *)&info, 0);
                }
            }
        }

        vTaskDelay(5);
    }
}
