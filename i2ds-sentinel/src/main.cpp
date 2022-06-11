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
<<<<<<< HEAD
#define FORMAT_SPIFFS_IF_FAILED true



static TaskHandle_t detector;
static TaskHandle_t ipc;

//static TaskHandle_t camera;
// Define the Firebase Data object
=======
#include "esp32-hal-psram.h"
#define FORMAT_SPIFFS_IF_FAILED true

static TaskHandle_t detector;
static TaskHandle_t ipc;

// static TaskHandle_t camera;
//  Define the Firebase Data object
>>>>>>> 6ad028a2c49c97937b8459cd1397009f2f004645

// Assign the project host and api key (required)
void setup()
{

    APP_LOG_START();
<<<<<<< HEAD
   
    ledcSetup(15, 5000, 8);
    ledcAttachPin(4, 15);
  

    xTaskCreate(detectorTask, "detector", 8192, NULL, 0, &detector);
    vTaskDelete(NULL);
=======
    psramInit();

    xTaskCreatePinnedToCore(detectorTask, "detector", 16384, NULL, 1, &detector, 1);
    vTaskDelete(NULL); 
>>>>>>> 6ad028a2c49c97937b8459cd1397009f2f004645
}

void loop()
{
<<<<<<< HEAD



        
    
=======
>>>>>>> 6ad028a2c49c97937b8459cd1397009f2f004645
}