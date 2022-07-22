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

void ledTask(void *pvParameters)
{
    ws2812fx.init();
    ws2812fx.setBrightness(75);
    ws2812fx.setSegment(0, 0, 9 - 1, 2, RED, 3000, NO_OPTIONS);
    ws2812fx.start();
    uiQueue = xQueueCreate(1, sizeof(uint8_t));
    uiCooldown = xTimerCreate("uiCooldown", 2500, pdTRUE, NULL, uiCooldownCallback);
    uiRunning = xTimerCreate("uiRunning", 225, pdTRUE, NULL, uiRunningCallback);
    ledcSetup(14, 5000, 8);
    ledcAttachPin(13, 14);
    while (1)
    {
        if (uxQueueMessagesWaiting(uiQueue) > 0)
        {

            if (xQueueReceive(uiQueue, &rec, 1) == pdPASS)
            {

                if ((rec == 128) && (xTimerIsTimerActive(uiCooldown) == pdFALSE) && (xTimerIsTimerActive(uiRunning) == pdFALSE))
                {
                    xTimerStart(uiRunning, 0);
                    ws2812fx.setSegment(0, 0, 9 - 1, 3, PURPLE, 300, NO_OPTIONS);
                    ws2812fx.start();
                }

                else if ((rec == 128) && (xTimerIsTimerActive(uiCooldown) == pdTRUE))
                    xTimerStart(uiCooldown, 0);
                else if (rec == 255)
                {
                    xTimerStart(uiRunning, 0);
                    ws2812fx.setBrightness(255);
                    ws2812fx.setSegment(0, 0, 4 - 1, 41, RED, 300, NO_OPTIONS);
                    ws2812fx.start();
                    ledcWriteNote(14, NOTE_Eb, 2);
                }
            }
        }

        ws2812fx.service();
        vTaskDelay(5);
    }
}