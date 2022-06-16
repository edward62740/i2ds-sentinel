#include <Arduino.h>
#include "app_firebase.h"
#include "app_detector.h"
#include "SPIFFS.h"
#include "FS.h"
#include "app_ipc.h"
#include "app_common.h"
#include "esp32-hal-psram.h"


#define FORMAT_SPIFFS_IF_FAILED true

static TaskHandle_t ipc;
static TaskHandle_t firebase;
void setup()
{
    APP_LOG_START();
    psramInit();
    SPIFFS.begin();
    xTaskCreatePinnedToCore(firebaseTask, "firebase", 8192, NULL, 1, &firebase, 0);
   // xTaskCreatePinnedToCore(ipcTask, "ipc", 8192, NULL, 1, &ipc, 1);
    vTaskDelete(NULL);
}

void loop()
{
}