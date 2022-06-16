#include <Arduino.h>
#include "app_common.h"
#include "app_tripwire.h"
#include "vl53l1x.h"

VL53L1X vl53l1x;

void tripwireTask(void *pvParameters)
{
    vl53l1x.init(false);
}