#include <Arduino.h>
#define CAMERA_MODEL_AI_THINKER
#include "app_common.h"
#include "esp_camera.h"
#include "app_camera.h"
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

/*! AppCamera Constructor
    @brief Constructor
    @param void
    @return void
*/
AppCamera::AppCamera() {}

/*! init()
   @brief initializes camera to jpeg mode
   @note
   @param void
   @return void
*/
bool AppCamera::init(pixformat_t pixformat)
{
  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 16000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;
  config.frame_size = FRAMESIZE_96X96;
  config.jpeg_quality = 4;
  config.fb_count = 1;
  esp_camera_init(&config);
  return true;
}

/*! captureGreyscale()
   @brief captures greyscale image to be fed into detector, and sets the camera to jpeg mode afterwards
   @note
   @param void
   @return pointer to frame buffer
*/
uint8_t *AppCamera::captureGreyscale()
{
  camera_fb_t *frame;
  frame = esp_camera_fb_get();
  esp_camera_fb_return(frame);
  return frame->buf;
}

/*! captureJpeg()
   @brief captures jpeg image and saves to SPIFFS, and sets the camera to greyscale mode afterwards
   @note
   @param void
   @return void
*/
void AppCamera::captureJpeg(bool reinitAsGreyscale = true)
{
  
 
  camera_fb_t *frame;

  frame = esp_camera_fb_get();

}

/*! ~AppCamera()
   @brief destructor
   @note
   @param void
   @return void
*/
AppCamera::~AppCamera() {}
