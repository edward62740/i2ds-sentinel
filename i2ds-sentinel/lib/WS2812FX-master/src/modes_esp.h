/*
  modes_esp.h - WS2812FX header file for ESP8266 and ESP32 microprocessors

  LICENSE

  The MIT License (MIT)

  Copyright (c) 2016  Harm Aldick

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.


  CHANGELOG

  2022-03-23   Separated from the original WS2812FX.h file
*/
#ifndef mode_esp_h
#define mode_esp_h

#define MODE_COUNT (sizeof(_modes)/sizeof(_modes[0]))
#define MODE_PTR(x) this->*_modes[x].mode_ptr
#define MODE_NAME(x) _modes[x].name

#define FX_MODE_STATIC                   0
#define FX_MODE_BLINK                    1
#define FX_MODE_BREATH                   2
#define FX_MODE_COLOR_WIPE               3
#define FX_MODE_COLOR_WIPE_INV           4 
#define FX_MODE_COLOR_WIPE_REV           5
#define FX_MODE_COLOR_WIPE_REV_INV       6
#define FX_MODE_COLOR_WIPE_RANDOM        7
#define FX_MODE_RANDOM_COLOR             8
#define FX_MODE_SINGLE_DYNAMIC           9
#define FX_MODE_MULTI_DYNAMIC           10
#define FX_MODE_RAINBOW                 11
#define FX_MODE_RAINBOW_CYCLE           12
#define FX_MODE_SCAN                    13
#define FX_MODE_DUAL_SCAN               14
#define FX_MODE_FADE                    15
#define FX_MODE_THEATER_CHASE           16
#define FX_MODE_THEATER_CHASE_RAINBOW   17
#define FX_MODE_RUNNING_LIGHTS          18
#define FX_MODE_TWINKLE                 19
#define FX_MODE_TWINKLE_RANDOM          20
#define FX_MODE_TWINKLE_FADE            21
#define FX_MODE_TWINKLE_FADE_RANDOM     22
#define FX_MODE_SPARKLE                 23
#define FX_MODE_FLASH_SPARKLE           24
#define FX_MODE_HYPER_SPARKLE           25
#define FX_MODE_STROBE                  26
#define FX_MODE_STROBE_RAINBOW          27
#define FX_MODE_MULTI_STROBE            28
#define FX_MODE_BLINK_RAINBOW           29
#define FX_MODE_CHASE_WHITE             30
#define FX_MODE_CHASE_COLOR             31
#define FX_MODE_CHASE_RANDOM            32
#define FX_MODE_CHASE_RAINBOW           33
#define FX_MODE_CHASE_FLASH             34
#define FX_MODE_CHASE_FLASH_RANDOM      35
#define FX_MODE_CHASE_RAINBOW_WHITE     36
#define FX_MODE_CHASE_BLACKOUT          37
#define FX_MODE_CHASE_BLACKOUT_RAINBOW  38
#define FX_MODE_COLOR_SWEEP_RANDOM      39
#define FX_MODE_RUNNING_COLOR           40
#define FX_MODE_RUNNING_RED_BLUE        41
#define FX_MODE_RUNNING_RANDOM          42
#define FX_MODE_LARSON_SCANNER          43
#define FX_MODE_COMET                   44
#define FX_MODE_FIREWORKS               45
#define FX_MODE_FIREWORKS_RANDOM        46
#define FX_MODE_MERRY_CHRISTMAS         47
#define FX_MODE_FIRE_FLICKER            48
#define FX_MODE_FIRE_FLICKER_SOFT       49
#define FX_MODE_FIRE_FLICKER_INTENSE    50
#define FX_MODE_CIRCUS_COMBUSTUS        51
#define FX_MODE_HALLOWEEN               52
#define FX_MODE_BICOLOR_CHASE           53
#define FX_MODE_TRICOLOR_CHASE          54
#define FX_MODE_TWINKLEFOX              55
#define FX_MODE_RAIN                    56
#define FX_MODE_CUSTOM                  57  // keep this for backward compatiblity
#define FX_MODE_CUSTOM_0                57  // custom modes need to go at the end
#define FX_MODE_CUSTOM_1                58
#define FX_MODE_CUSTOM_2                59
#define FX_MODE_CUSTOM_3                60
#define FX_MODE_CUSTOM_4                61
#define FX_MODE_CUSTOM_5                62
#define FX_MODE_CUSTOM_6                63
#define FX_MODE_CUSTOM_7                64

typedef struct Mode {
  const __FlashStringHelper* name;
  const __FlashStringHelper* category;
  uint16_t (WS2812FX::*mode_ptr)(void);
} mode;

// mode categories
const char cat_simple[]  EXT_RAM_ATTR  = "Simple";
const char cat_wipe[]    EXT_RAM_ATTR  = "Wipe";
const char cat_sweep[]   EXT_RAM_ATTR  = "Sweep";
const char cat_special[] EXT_RAM_ATTR  = "Special";
const char cat_custom[]  EXT_RAM_ATTR  = "Custom";

// create GLOBAL names to allow WS2812FX to compile with sketches and other libs
// that store strings in EXT_RAM_ATTR  (get rid of the "section type conflict with __c"
// errors once and for all. Amen.)
const char name_0[] EXT_RAM_ATTR  = "Static";
const char name_1[] EXT_RAM_ATTR  = "Blink";
const char name_2[] EXT_RAM_ATTR  = "Breath";
const char name_3[] EXT_RAM_ATTR  = "Color Wipe";
const char name_4[] EXT_RAM_ATTR  = "Color Wipe Inverse";
const char name_5[] EXT_RAM_ATTR  = "Color Wipe Reverse";
const char name_6[] EXT_RAM_ATTR  = "Color Wipe Reverse Inverse";
const char name_7[] EXT_RAM_ATTR  = "Color Wipe Random";
const char name_8[] EXT_RAM_ATTR  = "Random Color";
const char name_9[] EXT_RAM_ATTR  = "Single Dynamic";
const char name_10[] EXT_RAM_ATTR  = "Multi Dynamic";
const char name_11[] EXT_RAM_ATTR  = "Rainbow";
const char name_12[] EXT_RAM_ATTR  = "Rainbow Cycle";
const char name_13[] EXT_RAM_ATTR  = "Scan";
const char name_14[] EXT_RAM_ATTR  = "Dual Scan";
const char name_15[] EXT_RAM_ATTR  = "Fade";
const char name_16[] EXT_RAM_ATTR  = "Theater Chase";
const char name_17[] EXT_RAM_ATTR  = "Theater Chase Rainbow";
const char name_18[] EXT_RAM_ATTR  = "Running Lights";
const char name_19[] EXT_RAM_ATTR  = "Twinkle";
const char name_20[] EXT_RAM_ATTR  = "Twinkle Random";
const char name_21[] EXT_RAM_ATTR  = "Twinkle Fade";
const char name_22[] EXT_RAM_ATTR  = "Twinkle Fade Random";
const char name_23[] EXT_RAM_ATTR  = "Sparkle";
const char name_24[] EXT_RAM_ATTR  = "Flash Sparkle";
const char name_25[] EXT_RAM_ATTR  = "Hyper Sparkle";
const char name_26[] EXT_RAM_ATTR  = "Strobe";
const char name_27[] EXT_RAM_ATTR  = "Strobe Rainbow";
const char name_28[] EXT_RAM_ATTR  = "Multi Strobe";
const char name_29[] EXT_RAM_ATTR  = "Blink Rainbow";
const char name_30[] EXT_RAM_ATTR  = "Chase White";
const char name_31[] EXT_RAM_ATTR  = "Chase Color";
const char name_32[] EXT_RAM_ATTR  = "Chase Random";
const char name_33[] EXT_RAM_ATTR  = "Chase Rainbow";
const char name_34[] EXT_RAM_ATTR  = "Chase Flash";
const char name_35[] EXT_RAM_ATTR  = "Chase Flash Random";
const char name_36[] EXT_RAM_ATTR  = "Chase Rainbow White";
const char name_37[] EXT_RAM_ATTR  = "Chase Blackout";
const char name_38[] EXT_RAM_ATTR  = "Chase Blackout Rainbow";
const char name_39[] EXT_RAM_ATTR  = "Color Sweep Random";
const char name_40[] EXT_RAM_ATTR  = "Running Color";
const char name_41[] EXT_RAM_ATTR  = "Running Red Blue";
const char name_42[] EXT_RAM_ATTR  = "Running Random";
const char name_43[] EXT_RAM_ATTR  = "Larson Scanner";
const char name_44[] EXT_RAM_ATTR  = "Comet";
const char name_45[] EXT_RAM_ATTR  = "Fireworks";
const char name_46[] EXT_RAM_ATTR  = "Fireworks Random";
const char name_47[] EXT_RAM_ATTR  = "Merry Christmas";
const char name_48[] EXT_RAM_ATTR  = "Fire Flicker";
const char name_49[] EXT_RAM_ATTR  = "Fire Flicker (soft)";
const char name_50[] EXT_RAM_ATTR  = "Fire Flicker (intense)";
const char name_51[] EXT_RAM_ATTR  = "Circus Combustus";
const char name_52[] EXT_RAM_ATTR  = "Halloween";
const char name_53[] EXT_RAM_ATTR  = "Bicolor Chase";
const char name_54[] EXT_RAM_ATTR  = "Tricolor Chase";
const char name_55[] EXT_RAM_ATTR  = "TwinkleFOX";
const char name_56[] EXT_RAM_ATTR  = "Rain";
const char name_57[] EXT_RAM_ATTR  = "Custom 0"; // custom modes need to go at the end
const char name_58[] EXT_RAM_ATTR  = "Custom 1";
const char name_59[] EXT_RAM_ATTR  = "Custom 2";
const char name_60[] EXT_RAM_ATTR  = "Custom 3";
const char name_61[] EXT_RAM_ATTR  = "Custom 4";
const char name_62[] EXT_RAM_ATTR  = "Custom 5";
const char name_63[] EXT_RAM_ATTR  = "Custom 6";
const char name_64[] EXT_RAM_ATTR  = "Custom 7";

// define static array of member function pointers.
// make sure the order of the _modes array elements matches the FX_MODE_* values
__attribute__ ((unused)) static mode _modes[] = {
  { FSH(name_0),  FSH(cat_simple),  &WS2812FX::mode_static },
  { FSH(name_1),  FSH(cat_simple),  &WS2812FX::mode_blink },
  { FSH(name_2),  FSH(cat_special), &WS2812FX::mode_breath },
  { FSH(name_3),  FSH(cat_wipe),    &WS2812FX::mode_color_wipe },
  { FSH(name_4),  FSH(cat_wipe),    &WS2812FX::mode_color_wipe_inv },
  { FSH(name_5),  FSH(cat_sweep),   &WS2812FX::mode_color_wipe_rev },
  { FSH(name_6),  FSH(cat_sweep),   &WS2812FX::mode_color_wipe_rev_inv },
  { FSH(name_7),  FSH(cat_wipe),    &WS2812FX::mode_color_wipe_random },
  { FSH(name_8),  FSH(cat_simple),  &WS2812FX::mode_random_color },
  { FSH(name_9),  FSH(cat_simple),  &WS2812FX::mode_single_dynamic },
  { FSH(name_10), FSH(cat_simple),  &WS2812FX::mode_multi_dynamic },
  { FSH(name_11), FSH(cat_simple),  &WS2812FX::mode_rainbow },
  { FSH(name_12), FSH(cat_wipe),    &WS2812FX::mode_rainbow_cycle },
  { FSH(name_13), FSH(cat_sweep),   &WS2812FX::mode_scan },
  { FSH(name_14), FSH(cat_sweep),   &WS2812FX::mode_dual_scan },
  { FSH(name_15), FSH(cat_sweep),   &WS2812FX::mode_fade },
  { FSH(name_16), FSH(cat_wipe),    &WS2812FX::mode_theater_chase },
  { FSH(name_17), FSH(cat_wipe),    &WS2812FX::mode_theater_chase_rainbow },
  { FSH(name_18), FSH(cat_wipe),    &WS2812FX::mode_running_lights },
  { FSH(name_19), FSH(cat_simple),  &WS2812FX::mode_twinkle },
  { FSH(name_20), FSH(cat_simple),  &WS2812FX::mode_twinkle_random },
  { FSH(name_21), FSH(cat_simple),  &WS2812FX::mode_twinkle_fade },
  { FSH(name_22), FSH(cat_simple),  &WS2812FX::mode_twinkle_fade_random },
  { FSH(name_23), FSH(cat_simple),  &WS2812FX::mode_sparkle },
  { FSH(name_24), FSH(cat_simple),  &WS2812FX::mode_flash_sparkle },
  { FSH(name_25), FSH(cat_simple),  &WS2812FX::mode_hyper_sparkle },
  { FSH(name_26), FSH(cat_simple),  &WS2812FX::mode_strobe },
  { FSH(name_27), FSH(cat_simple),  &WS2812FX::mode_strobe_rainbow },
  { FSH(name_28), FSH(cat_simple),  &WS2812FX::mode_multi_strobe },
  { FSH(name_29), FSH(cat_simple),  &WS2812FX::mode_blink_rainbow },
  { FSH(name_30), FSH(cat_wipe),    &WS2812FX::mode_chase_white },
  { FSH(name_31), FSH(cat_wipe),    &WS2812FX::mode_chase_color },
  { FSH(name_32), FSH(cat_wipe),    &WS2812FX::mode_chase_random },
  { FSH(name_33), FSH(cat_wipe),    &WS2812FX::mode_chase_rainbow },
  { FSH(name_34), FSH(cat_wipe),    &WS2812FX::mode_chase_flash },
  { FSH(name_35), FSH(cat_wipe),    &WS2812FX::mode_chase_flash_random },
  { FSH(name_36), FSH(cat_wipe),    &WS2812FX::mode_chase_rainbow_white },
  { FSH(name_37), FSH(cat_wipe),    &WS2812FX::mode_chase_blackout },
  { FSH(name_38), FSH(cat_wipe),    &WS2812FX::mode_chase_blackout_rainbow },
  { FSH(name_39), FSH(cat_sweep),   &WS2812FX::mode_color_sweep_random },
  { FSH(name_40), FSH(cat_wipe),    &WS2812FX::mode_running_color },
  { FSH(name_41), FSH(cat_wipe),    &WS2812FX::mode_running_red_blue },
  { FSH(name_42), FSH(cat_wipe),    &WS2812FX::mode_running_random },
  { FSH(name_43), FSH(cat_sweep),   &WS2812FX::mode_larson_scanner },
  { FSH(name_44), FSH(cat_wipe),    &WS2812FX::mode_comet },
  { FSH(name_45), FSH(cat_simple),  &WS2812FX::mode_fireworks },
  { FSH(name_46), FSH(cat_simple),  &WS2812FX::mode_fireworks_random },
  { FSH(name_47), FSH(cat_wipe),    &WS2812FX::mode_merry_christmas },
  { FSH(name_48), FSH(cat_simple),  &WS2812FX::mode_fire_flicker },
  { FSH(name_49), FSH(cat_simple),  &WS2812FX::mode_fire_flicker_soft },
  { FSH(name_50), FSH(cat_simple),  &WS2812FX::mode_fire_flicker_intense },
  { FSH(name_51), FSH(cat_wipe),    &WS2812FX::mode_circus_combustus },
  { FSH(name_52), FSH(cat_wipe),    &WS2812FX::mode_halloween },
  { FSH(name_53), FSH(cat_wipe),    &WS2812FX::mode_bicolor_chase },
  { FSH(name_54), FSH(cat_wipe),    &WS2812FX::mode_tricolor_chase },
  { FSH(name_55), FSH(cat_special), &WS2812FX::mode_twinkleFOX },
  { FSH(name_56), FSH(cat_special), &WS2812FX::mode_rain },
  { FSH(name_57), FSH(cat_custom),  &WS2812FX::mode_custom_0 },
  { FSH(name_58), FSH(cat_custom),  &WS2812FX::mode_custom_1 },
  { FSH(name_59), FSH(cat_custom),  &WS2812FX::mode_custom_2 },
  { FSH(name_60), FSH(cat_custom),  &WS2812FX::mode_custom_3 },
  { FSH(name_61), FSH(cat_custom),  &WS2812FX::mode_custom_4 },
  { FSH(name_62), FSH(cat_custom),  &WS2812FX::mode_custom_5 },
  { FSH(name_63), FSH(cat_custom),  &WS2812FX::mode_custom_6 },
  { FSH(name_64), FSH(cat_custom),  &WS2812FX::mode_custom_7 }
};
#endif
