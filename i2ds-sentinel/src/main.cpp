#include <Arduino.h>
#include "app_firebase.h"
#include "app_detector.h"
#include "app_ipc.h"
#include "app_common.h"
#include "esp32-hal-psram.h"
static TaskHandle_t ipc;
static TaskHandle_t firebase;
void setup()
{
    APP_LOG_START();
     vTaskDelay(1000);
    while(!psramInit())
    {
        APP_LOG_INFO("[INIT] ERROR INITIALIZING PSRAM");
        vTaskDelay(1000);
    }
    xTaskCreatePinnedToCore(firebaseTask, "firebase", 16384, NULL, 1, &firebase, 0);
    xTaskCreatePinnedToCore(ipcTask, "ipc", 8192, NULL, 1, &ipc, 0);
    vTaskDelete(NULL);
}

void loop()
{
}