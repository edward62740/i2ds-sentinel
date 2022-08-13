#include <Arduino.h>
#include "app_common.h"
#include "app_ui.h"
#include "WS2812FX.h"
#include "Adafruit_NeoPixel.h"
#include "WS2812FX.h"
#include "Adafruit_NeoPixel.h"
QueueHandle_t uiQueue;
TimerHandle_t uiCooldown;
TimerHandle_t uiRunning;
WS2812FX ws2812fx = WS2812FX(9, 2, NEO_GRB + NEO_KHZ800);
uint8_t rec = 0;

void uiCooldownCallback(TimerHandle_t uiCooldown)
{
    ws2812fx.setBrightness(75);
    ws2812fx.setSegment(0, 0, 9 - 1, 2, RED, 3000, NO_OPTIONS);
    ws2812fx.start();
    xTimerStop(uiCooldown, 0);
    ledcWrite(14, 0);
}

void uiRunningCallback(TimerHandle_t uiRunning)
{
    if (rec == 255)
    {
        ws2812fx.setBrightness(255);
        ws2812fx.setSegment(0, 0, 4 - 1, 41, RED, 300, NO_OPTIONS);
        ws2812fx.start();
        if (!MUTE_DEVICE)
            ledcWriteNote(14, NOTE_Eb, 2);
    }
    else if (rec == 128)
    {
        ws2812fx.setBrightness(25);
        ws2812fx.setSegment(0, 0, 9 - 1, 12, RED, 500, NO_OPTIONS);
        ws2812fx.start();
    }

    xTimerStart(uiCooldown, 0);
    xTimerStop(uiRunning, 0);
}

void uiTask(void *pvParameters)
{
    ws2812fx.init();
    ws2812fx.setBrightness(75);
    ws2812fx.setSegment(0, 0, 9 - 1, 2, RED, 3000, NO_OPTIONS);
    ws2812fx.start();
    uiQueue = xQueueCreate(2, sizeof(uint8_t));
    uiCooldown = xTimerCreate("uiCooldown", AUX_LED_DETPOS_CYCLE_MS, pdTRUE, NULL, uiCooldownCallback);
    uiRunning = xTimerCreate("uiRunning", AUX_LED_DETPOS_FLASH_MS, pdTRUE, NULL, uiRunningCallback);
    ledcSetup(14, 5000, 8);
    ledcAttachPin(13, 14);
    while (1)
    {
        if (uxQueueMessagesWaiting(uiQueue) > 0)
        {

            if (xQueueReceive(uiQueue, &rec, 1) == pdPASS)
            {
                /* Note:
                   rec should be set to 0 by DEFAULT
                   or 128 to indicate START of detector positive (passed by instance of detector in app_main.cpp) at interval exceeding AUX_LED_DETPOS_CYCLE_MS for continuity
                   or 255 to indicate START of warning from ipc
                   or 122 to indicate START of warning from tripwire
                   END condition is handled internally here.
                */
                if ((rec == 128) && (xTimerIsTimerActive(uiCooldown) == pdFALSE) && (xTimerIsTimerActive(uiRunning) == pdFALSE))
                {
                    /* Flash purple before transition to rainbow */
                    xTimerStart(uiRunning, 0);
                    ws2812fx.setSegment(0, 0, 9 - 1, 3, PURPLE, 300, NO_OPTIONS);
                    ws2812fx.start();
                }
                else if ((rec == 128) && (xTimerIsTimerActive(uiCooldown) == pdTRUE))
                    /* Reload timer if already in cooldown mode */
                    xTimerStart(uiCooldown, 0);
                else if (rec == 255)
                {
                    /* Flash red for warning from ipc and turn on buzzers */
                    xTimerStart(uiRunning, 0);
                    ws2812fx.setBrightness(255);
                    ws2812fx.setSegment(0, 0, 4 - 1, 41, RED, 300, NO_OPTIONS);
                    ws2812fx.start();
                    if (!MUTE_DEVICE)
                        ledcWriteNote(14, NOTE_Eb, 5);
                }
                else if (rec == 122)
                {
                    xTimerStart(uiCooldown, 0);
                    ws2812fx.setSegment(0, 0, 9 - 1, 1, WHITE, 300, NO_OPTIONS);
                    ws2812fx.start();
                }
            }
        }
        ws2812fx.service();
        vTaskDelay(5);
    }
}