#include <EloquentVision.h>
#include "SPIFFS.h"
#include "app_common.h"
Eloquent::Vision::ESP32Camera camera;
camera_fb_t *frame;
camera_fb_t *frame2;
/**
 * Configure camera
 */
void initCamera()
{
  Serial.println("CAMERA INIT STARTED");

  Serial.println("CAMERA INIT ENDED");
}

/**
 * Capture frame from ESP32 camera
 */
uint8_t *captureFrame()
{
  digitalWrite(PWDN_GPIO_NUM, HIGH);
    digitalWrite(RESET_GPIO_NUM, HIGH);
  delay(50);
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

  bool ok = esp_camera_init(&config) == ESP_OK;
  delay(1);
  sensor_t *sensor = esp_camera_sensor_get();
  sensor->set_framesize(sensor, FRAMESIZE_QQVGA);
  sensor->set_vflip(sensor, 1);

  Serial.println("CAMERA CAPTURE STARTED");
  frame = esp_camera_fb_get();
  Serial.println("CAMERA CAPTURE ENDED");
  esp_camera_deinit();
  delay(50);
  digitalWrite(PWDN_GPIO_NUM, LOW);
    digitalWrite(RESET_GPIO_NUM, LOW);
  return frame->buf;
}

void getFrameJpeg()
{
  digitalWrite(PWDN_GPIO_NUM, HIGH);
  digitalWrite(RESET_GPIO_NUM, HIGH);
  delay(50);
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
  config.jpeg_quality = 15;
  config.fb_count = 1;

  bool ok = esp_camera_init(&config) == ESP_OK;
  sensor_t *sensor = esp_camera_sensor_get();
  sensor->set_vflip(sensor, 1);

  delay(1);

  frame2 = esp_camera_fb_get();

  File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file in writing mode");
  }
  else
  {
    file.write(frame2->buf, frame2->len); // payload (image), payload length
    Serial.print("The picture has been saved in ");
    Serial.print(FILE_PHOTO);
    Serial.print(" - Size: ");
    Serial.print(frame2->len);
    Serial.println(" bytes");
  }

  file.close();
  esp_camera_deinit();
  delay(50);
  digitalWrite(PWDN_GPIO_NUM, LOW);
    digitalWrite(RESET_GPIO_NUM, LOW);
}