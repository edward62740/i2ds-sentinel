#include <Arduino.h>
#define CAMERA_MODEL_AI_THINKER
#include <EloquentVision.h>
#include "SPIFFS.h"
#include "app_common.h"
#include "esp_camera.h"
#include "app_camera.h"
camera_fb_t *frame;
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
  config.xclk_freq_hz = 10000000;
  if (pixformat == PIXFORMAT_JPEG)
  {
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;
  }
  else if (pixformat == PIXFORMAT_GRAYSCALE)
  {
    config.pixel_format = PIXFORMAT_GRAYSCALE;
    config.frame_size = FRAMESIZE_QQVGA;
    config.jpeg_quality = 4;
    config.fb_count = 1;
  }
  else
    return false;
  esp_camera_init(&config);
  return true;
}

/*! captureGreyscale()
   @brief captures greyscale image to be fed into detector, and sets the camera to jpeg mode afterwards
   @note
   @param void
   @return pointer to frame buffer
*/
uint8_t *AppCamera::captureGreyscale(bool reinitAsJpeg)
{
  returnBuffer();
  frame = esp_camera_fb_get();
  if (reinitAsJpeg)
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
    config.xclk_freq_hz = 10000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;
    esp_camera_deinit();
    digitalWrite(PWDN_GPIO_NUM, LOW);
    delay(50);
    digitalWrite(PWDN_GPIO_NUM, HIGH);
    delay(100);
    esp_camera_init(&config);
  }
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
  returnBuffer();
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
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;
  config.frame_size = FRAMESIZE_QQVGA;
  config.jpeg_quality = 4;
  config.fb_count = 1;
  frame = esp_camera_fb_get();
  File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file in writing mode");
  }
  else
  {
    file.write(frame->buf, frame->len); // payload (image), payload length
    Serial.print("The picture has been saved in ");
    Serial.print(FILE_PHOTO);
    Serial.print(" - Size: ");
    Serial.print(frame->len);
    Serial.println(" bytes");
  }

  file.close();
  if (reinitAsGreyscale)
  {
    esp_camera_deinit();
    digitalWrite(PWDN_GPIO_NUM, LOW);
    delay(50);
    digitalWrite(PWDN_GPIO_NUM, HIGH);
    delay(100);
    esp_camera_init(&config);
  }
}
/*! returnBuffer()
   @brief call to return the frame buffer to the camera when done reading it
   @note
   @param void
   @return void
*/
void AppCamera::returnBuffer()
{
  esp_camera_fb_return(frame);
  vTaskDelay(1);
}
/*! ~AppCamera()
   @brief destructor
   @note
   @param void
   @return void
*/
AppCamera::~AppCamera() {}
