#include <Arduino.h>
#include "app_common.h"
#include "app_tripwire.h"
#include "app_ui.h"
#include "vl53l1x.h"
#include "LIS2DW12.h"

VL53L1X tw;
LIS2DW12 IMU(I2C_MODE, 0x19, SPISettings(0, MSBFIRST, SPI_MODE0));
TimerHandle_t tripwireTimer;
void tripwireTimerCallback(TimerHandle_t tripwireTimer)
{
    uint8_t val = 122;
    xQueueSend(uiQueue, (void *)&val, 0);
    APP_LOG_INFO("FLASH LED");
}

void tripwireTask(void *pvParameters)
{
    tripwireTimer = xTimerCreate("tripwireTimer", 1000, pdFALSE, NULL, tripwireTimerCallback);
    tw.setTimeout(500);
    IMU.begin();
    if (!tw.init())
    {
        Serial.println("Failed to detect and initialize tw!");
        while (1)
            ;
    }

    // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
    // You can change these settings to adjust the performance of the tw, but
    // the minimum timing budget is 20 ms for short distance mode and 33 ms for
    // medium and long distance modes. See the VL53L1X datasheet for more
    // information on range and timing limits.
    tw.setDistanceMode(VL53L1X::Long);
    tw.setMeasurementTimingBudget(200000);
    tw.setROISize(8, 8);
    tw.startContinuous(250);
    while (1)
    {
        tw.read();
        if ((tw.ranging_data.range_mm < 90 && tw.ranging_data.range_mm > 45) && xTimerIsTimerActive(tripwireTimer) == pdFALSE)
        {
            xTimerStart(tripwireTimer, 0);
        }
        else if (tw.ranging_data.range_mm >= 90 || tw.ranging_data.range_mm <= 45)
        {
            xTimerStop(tripwireTimer, 0);
        }
        vTaskDelay(50);
    }
}