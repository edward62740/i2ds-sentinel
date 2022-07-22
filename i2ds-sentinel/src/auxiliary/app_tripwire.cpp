#include <Arduino.h>
#include "app_common.h"
#include "app_tripwire.h"
#include "vl53l1x.h"
#include "LIS2DW12.h"

VL53L1X tw;
LIS2DW12 IMU( I2C_MODE, 0x19, SPISettings(0, MSBFIRST, SPI_MODE0) );

void tripwireTask(void *pvParameters)
{
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
    tw.setMeasurementTimingBudget(15000);
    tw.setROISize(8, 8);
    //tw.setROICenter(87);
    // Start continuous readings at a rate of one measurement every 50 ms (the
    // inter-measurement period). This period should be at least as long as the
    // timing budget.
    tw.startContinuous(15);
    while (1)
    {
        tw.read();
/*
        Serial.print("range: ");
        Serial.print(tw.ranging_data.range_mm);
        Serial.print("\tstatus: ");
        Serial.print(VL53L1X::rangeStatusToString(tw.ranging_data.range_status));
        Serial.print("\tpeak signal: ");
        Serial.print(tw.ranging_data.peak_signal_count_rate_MCPS);
        Serial.print("\tambient: ");
        Serial.print(tw.ranging_data.ambient_count_rate_MCPS);

        Serial.println();*/
        vTaskDelay(50);
    }
}