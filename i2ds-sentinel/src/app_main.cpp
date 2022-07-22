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

String devicename = "I2DS SENTINEL";
AppCamera appCam((uint8_t)IR_LED);
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
/* Pass context to app detector object */
detector_ctx_t det_ctx;
AppDetector appDet(&det_ctx);
TaskHandle_t dt;
QueueHandle_t ipcQueue;
TimerHandle_t ipcWarningRateLimiter;
TimerHandle_t firebaseUpdateTimer;
QueueHandle_t firebaseSendPayloadQueue;
QueueHandle_t firebasePostQueue;
bool updateFirebase = false;

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

void fcsUploadCallback(FCS_UploadStatusInfo info)
{
    if (info.status == fb_esp_fcs_upload_status_init)
    {
        Serial.printf("Uploading file %s (%d) to %s\n", info.localFileName.c_str(), info.fileSize, info.remoteFileName.c_str());
    }
    else if (info.status == fb_esp_fcs_upload_status_upload)
    {
        Serial.printf("Uploaded %d%s, Elapsed time %d ms\n", (int)info.progress, "%", info.elapsedTime);
    }
    else if (info.status == fb_esp_fcs_upload_status_complete)
    {
        Serial.println("Upload completed\n");
        FileMetaInfo meta = fbdo.metaData();
        Serial.printf("Name: %s\n", meta.name.c_str());
        Serial.printf("Bucket: %s\n", meta.bucket.c_str());
        Serial.printf("contentType: %s\n", meta.contentType.c_str());
        Serial.printf("Size: %d\n", meta.size);
        Serial.printf("Generation: %lu\n", meta.generation);
        Serial.printf("Metageneration: %lu\n", meta.metageneration);
        Serial.printf("ETag: %s\n", meta.etag.c_str());
        Serial.printf("CRC32: %s\n", meta.crc32.c_str());
        Serial.printf("Token: %s\n", meta.downloadTokens.c_str());
        Serial.printf("Download URL: %s\n\n", fbdo.downloadURL().c_str());
    }
    else if (info.status == fb_esp_fcs_upload_status_error)
    {
        Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
    }
}

void appMainTask(void *pvParameters)
{

    esp_task_wdt_init(15, true);
    appDet.startDetectorTask(dt); // Start detector task with task handle dt
    ipcQueue = xQueueCreate(2, sizeof(ipc_warn_t));
    ipcWarningRateLimiter = xTimerCreate("ipcWarningRateLimiter", pdMS_TO_TICKS(5500), pdFALSE, NULL, NULL);
    firebaseUpdateTimer = xTimerCreate("firebaseUpdateTimer", pdMS_TO_TICKS(5000), pdTRUE, NULL, firebaseUpdateTimerCallback);
    firebaseSendPayloadQueue = xQueueCreate(2, sizeof(img_info_t));
    firebasePostQueue = xQueueCreate(5, sizeof(img_info_t));
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

    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(FIREBASE_DATA_OBJECT_PAYLOAD_SIZE_BYTES);
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    config.fcs.upload_buffer_size = 8192;
    config.tcp_data_sending_retry = 5;
    config.timeout.wifiReconnect = 10 * 1000;
    config.timeout.socketConnection = 6 * 1000;
    config.timeout.serverResponse = 8 * 1000;
    config.timeout.rtdbKeepAlive = 30 * 1000;
    config.timeout.rtdbStreamReconnect = 1 * 1000;
    config.timeout.rtdbStreamError = 3 * 1000;
    Firebase.signUp(&config, &auth, "", "");
    config.token_status_callback = tokenStatusCallback;
    Firebase.begin(&config, &auth);

    if (appCam.init() != ESP_OK)
    {
        APP_LOG_INFO("[INIT] ERROR INITIALIZING CAMERA");
        vTaskDelay(1000);
    }

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

        if (updateFirebase)
        {
            FirebaseJson selfInfoJson;
            String device = "/sentinel";
            selfInfoJson.set("timestamp/.sv", "timestamp");
            selfInfoJson.add("trigd", "test");
            while (uxQueueMessagesWaiting(firebasePostQueue) > 0)
            {
                img_info_t info;
                if (xQueueReceive(firebasePostQueue, &info, 1) == pdPASS)
                {
                    FirebaseJson imgInfoJson;
                    imgInfoJson.set("Source Node", info.ipc_info.src_id);
                    imgInfoJson.set("RSSI", info.ipc_info.rssi);
                    imgInfoJson.set("LQI", info.ipc_info.lqi);
                    imgInfoJson.set("Timestamp/.sv", "timestamp");
                    selfInfoJson.set((String)info.index, imgInfoJson);
                }
            }
            APP_LOG_INFO("[INIT] UPDATING FIREBASE");
            if (!Firebase.RTDB.updateNode(&fbdo, device, &selfInfoJson))
            {
                APP_LOG_ERR(fbdo.errorReason().c_str());
            }
            updateFirebase = false;
        }
        /* Detector indicates it is ready by giving the semaphore */
        if (xSemaphoreTake(det_ctx.send, 1) == pdTRUE)
        {
            /* Log results of detection */
            if (det_ctx.result.success)
            {
                if (det_ctx.result.detected)
                    APP_LOG_INFO("[DETECTOR] DETECTED");
                else
                    APP_LOG_INFO("[DETECTOR] NOT DETECTED");
                APP_LOG_INFO("[DETECTOR] Person detected score: " + (String)det_ctx.result.posConfidence);
                APP_LOG_INFO("[DETECTOR] No person detected score: " + (String)det_ctx.result.negConfidence);
            }
            else
                APP_LOG_INFO("[DETECTOR] ERROR DETECTING");

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

        // APP_LOG_INFO(WiFi.RSSI());
        /* Handle IPC warning. Payload information is sent to Firebase RTDB /SENTINEL/last and image is sent to database */
        if (uxQueueMessagesWaiting(ipcQueue) > 0)
        {
            ipc_warn_t warn;
            if (xQueueReceive(ipcQueue, &warn, 1) == pdPASS)
            {
                APP_LOG_WARN("IPC WARNING STARTS HERE");
                APP_LOG_WARN(warn.src_id);
                APP_LOG_WARN(warn.rssi);
                APP_LOG_WARN(warn.lqi);

                APP_LOG_WARN("RATE LIMITER NOT RUNNING");
                String filename = appCam.switchToJPEG(warn);

                appCam.switchToGreyscale();
            }
        }

        if (uxQueueMessagesWaiting(firebaseSendPayloadQueue) > 0)
        {
            img_info_t info;
            if (xQueueReceive(firebaseSendPayloadQueue, &info, 1) == pdPASS)
            {
                if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */, info.path /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, info.path /* path of remote file stored in the bucket */, "image/jpeg" /* mime type */, fcsUploadCallback))
                {
                    Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
                    // todo spiffs remove file
                    info.send_done = true;
                    if (xQueueSendToFront(firebasePostQueue, (void *)&info, 0) != 0)
                    {
                    }
                }
                else
                {
                    Serial.println(fbdo.errorReason());
                    info.send_err = true;
                    xQueueSendToBack(firebaseSendPayloadQueue, (void *)&info, 0);
                }
            }
        }

        vTaskDelay(5);
    }
}
