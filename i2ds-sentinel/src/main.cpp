/*** For compatibility with Arduino framework. Main task in app_main.cpp ***/

#include <Arduino.h>
#include "app_main.h"
#include "app_detector.h"
#include "app_ipc.h"
#include "app_common.h"
#include "auxiliary/app_tripwire.h"
#include "auxiliary/app_ui.h"
#include "esp32-hal-psram.h"
#include "SPIFFS.h"
#include <Wire.h>

static TaskHandle_t ipc;
static TaskHandle_t app;
static TaskHandle_t tripwire;
static TaskHandle_t led;
void setup()
{
    APP_LOG_START();
    SPIFFS.begin();
    Wire.begin(15, 14, (uint32_t)400000);
    if (!MUTE_DEVICE)
    {
        ledcSetup(14, 5000, 8);
        ledcAttachPin(13, 14);
        ledcWriteNote(14, NOTE_C, 5);
        vTaskDelay(100);
        ledcWriteNote(14, NOTE_D, 4);
        vTaskDelay(100);
        ledcWriteNote(14, NOTE_F, 5);
        vTaskDelay(100);
        ledcWriteNote(14, NOTE_A, 4);
        vTaskDelay(100);
        ledcWriteNote(14, NOTE_B, 5);
        vTaskDelay(300);
        ledcWrite(14, 0);
    }

    /* Application main task starts here */
    xTaskCreatePinnedToCore(appMainTask, "app", 16384, NULL, 2, &app, 0);

    /* Start task to communicate with efr32fg23 coprocessor in RX ONLY MODE. */
    xTaskCreatePinnedToCore(ipcTask, "ipc", 8192, NULL, 2, &ipc, 0);
    Serial1.begin(EUSART_IPC_BAUD, SERIAL_8O1, 12, -1);

    /* Start task to handle ToF sensor */
    xTaskCreatePinnedToCore(tripwireTask, "tripwire", 8192, NULL, 1, &tripwire, 0);

    /* Start task to handle UI (i.e LEDs, buzzer, etc.) */
    xTaskCreatePinnedToCore(uiTask, "leds", 8192, NULL, 1, &led, 0);

    /* Delete setup task */
    vTaskDelete(NULL);
}

void loop()
{
}
