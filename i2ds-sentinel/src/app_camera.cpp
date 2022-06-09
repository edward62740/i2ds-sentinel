#include <Arduino.h>
#define CAMERA_MODEL_AI_THINKER
#include <EloquentVision.h>
#include "SPIFFS.h"
#include "app_common.h"
#include "esp_camera.h"
#include "app_camera.h"

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
void AppCamera::init()
{

  Serial.println("CAMERA INIT STARTED");
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

  esp_camera_init(&config);
  sensor_t *sensor = esp_camera_sensor_get();
  sensor->set_vflip(sensor, 1);
  Serial.println("CAMERA INIT ENDED");
}

/*! captureGreyscale()
   @brief captures greyscale image to be fed into detector, and sets the camera to jpeg mode afterwards
   @note
   @param void
   @return pointer to frame buffer
*/
uint8_t* AppCamera::captureGreyscale()
{
  camera_fb_t *frame;
  camera_config_t config;
  sensor_t *sensor = esp_camera_sensor_get();
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

  APP_LOG_INFO("[CAMERA] CAMERA CAPTURE STARTED");
  // digitalWrite(4, HIGH);
  frame = esp_camera_fb_get();
  // digitalWrite(4, LOW);
  esp_camera_fb_return(frame);
  APP_LOG_INFO("[CAMERA] CAMERA CAPTURE ENDED");
  esp_camera_deinit();
  digitalWrite(PWDN_GPIO_NUM, LOW);
  APP_LOG_INFO("[CAMERA] CAMERA DEINIT");
  delay(500);
  digitalWrite(PWDN_GPIO_NUM, HIGH);
  delay(100);
  esp_camera_init(&config);

  sensor->set_vflip(sensor, 1);

  APP_LOG_INFO("[CAMERA] CAMERA REINIT");

  return frame->buf;
}


/*! captureJpeg()
   @brief captures jpeg image and saves to SPIFFS, and sets the camera to greyscale mode afterwards
   @note
   @param void
   @return void
*/
void AppCamera::captureJpeg()
{
  camera_fb_t *frame;
  camera_config_t config;
  sensor_t *sensor = esp_camera_sensor_get();
  
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
  APP_LOG_INFO("[CAMERA] CAMERA CAPTURE STARTED");
  frame = esp_camera_fb_get();
  APP_LOG_INFO("[CAMERA] CAMERA CAPTURE ENDED");
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
  esp_camera_fb_return(frame);
  esp_camera_deinit();
  APP_LOG_INFO("[CAMERA] CAMERA DEINIT-N");
  digitalWrite(PWDN_GPIO_NUM, LOW);
  delay(500);
  digitalWrite(PWDN_GPIO_NUM, HIGH);
  APP_LOG_INFO("[CAMERA] CAMERA REINIT-N");
  delay(100);
  esp_camera_init(&config);
  sensor->set_vflip(sensor, 1);
}

/*! ~AppCamera()
   @brief destructor
   @note
   @param void
   @return void
*/
AppCamera::~AppCamera() {}