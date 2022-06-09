#include <Arduino.h>
#include "app_common.h"
#include "app_camera.h"
#include "app_detector.h"
#include "WS2812FX.h"
#include "Adafruit_NeoPixel.h"
#include "VL53L1X.h"
#include "LIS2DW12.h"
#include "FS.h"
#include "SPIFFS.h"
#include "esp32-hal-psram.h"
#define FORMAT_SPIFFS_IF_FAILED true

static TaskHandle_t detector;
static TaskHandle_t ipc;

// static TaskHandle_t camera;
//  Define the Firebase Data object

// Assign the project host and api key (required)
void setup()
{

    APP_LOG_START();
    psramInit();

    xTaskCreatePinnedToCore(detectorTask, "detector", 16384, NULL, 1, &detector, 1);
    vTaskDelete(NULL); 
}

void loop()
{
}