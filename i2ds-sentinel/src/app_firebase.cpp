#include <Arduino.h>
#include "app_firebase.h"
#include "app_detector.h"
#include "app_camera.h"
#include "app_common.h"
#include "app_ipc.h"
#include "Firebase_ESP_Client.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String devicename = "I2DS SENTINEL";
AppCamera appCam;

/* Pass context to app detector object */
detector_ctx_t det_ctx;
AppDetector appDet(&det_ctx);
TaskHandle_t dt;

void firebaseTask(void *pvParameters)
{
    appDet.startDetectorTask(dt); // Start detector task with task handle dt
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

    config.api_key = API_KEY;
    Firebase.reconnectWiFi(true);

    Firebase.signUp(&config, &auth, "", "");
    Firebase.begin(&config, &auth);

    ledcSetup(14, 5000, 8);
    ledcAttachPin(4, 14);
    appCam.init(PIXFORMAT_GRAYSCALE);
    delay(1000);
    while (1)
    {
        if (xSemaphoreTake(det_ctx.send, 1) == pdTRUE)
        {
            APP_LOG_INFO(det_ctx.result.detected ? "[DETECTOR] PERSON DETECTED" : "[DETECTOR] NO PERSON DETECTED");
            APP_LOG_INFO("[DETECTOR] Person detected score: " + (String)det_ctx.result.posConfidence);
            APP_LOG_INFO("[DETECTOR] No person detected score: " + (String)det_ctx.result.negConfidence);
            APP_LOG_INFO("SEMAPHORE TAKEN");
            if (det_ctx.result.posConfidence > det_ctx.result.negConfidence)
                ledcWrite(14, 2);
            else
                ledcWrite(14, 0);
            if (uxQueueSpacesAvailable(det_ctx.frame) == 0)
                xQueueReset(det_ctx.frame);
            uint8_t *frameBuf = appCam.captureGreyscale();
            APP_LOG_INFO("CAPTURED");
            if (xQueueSend(det_ctx.frame, (void *)&frameBuf, 0) != 0)
            {
                APP_LOG_INFO("SENT");
            }
        }
        vTaskDelay(50);
    }
}