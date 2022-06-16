#include <Arduino.h>
#include "app_common.h"
#include "app_ui.h"
#include "WS2812FX.h"
#include "Adafruit_NeoPixel.h"

WS2812FX ws2812fx = WS2812FX(10, 1, NEO_GRB + NEO_KHZ800);

void ledTask (void *pvParameters)
{
  ws2812fx.init();
  ws2812fx.setBrightness(100);
  ws2812fx.setSpeed(200);
  ws2812fx.setMode(FX_MODE_RAINBOW_CYCLE);
  ws2812fx.start();
}