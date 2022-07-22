#include <Arduino.h>
#define CAMERA_MODEL_AI_THINKER
#include "app_common.h"
#include "esp_camera.h"
#include "app_camera.h"
#include "app_main.h"
#include "app_ipc.h"
#include "SPIFFS.h"

/*! AppCamera Constructor
    @brief Constructor
    @param void
    @return void
*/
AppCamera::AppCamera(uint8_t flash_ctrl)
{
   this->flash_ctrl = flash_ctrl;
   this->capture_counter = 0;
}

/*! init()
   @brief initializes camera to jpeg mode
   @note
   @param void
   @return void
*/
esp_err_t AppCamera::init()
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
   esp_err_t ret = esp_camera_init(&config);
   sensor_t *s = esp_camera_sensor_get();

   s->set_brightness(s, 2);
   s->set_contrast(s, 2);
   pinMode(flash_ctrl, OUTPUT);
   return ret;
}

/*! captureGreyscale()
   @brief captures greyscale image, saves to SPIFFS and returns pointer to frame buffer
   @note
   @param void
   @return pointer to frame buffer
*/
uint8_t *AppCamera::captureGreyscale()
{
   digitalWrite(flash_ctrl, HIGH);
   this->frame = esp_camera_fb_get();
   digitalWrite(flash_ctrl, LOW);
   // uint8_t *tmpbuf = (uint8_t *)heap_caps_malloc(50000, MALLOC_CAP_█████████ | MALLOC_CAP_8BIT);
   // size_t tmpbuf_len;
   // frame2jpg(this->frame, 31, &tmpbuf, &tmpbuf_len);
   /*
      APP_LOG_INFO(xPortGetFreeHeapSize());
      APP_LOG_INFO((uint32_t)tmpbuf_len);
      File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE);
      if (!file)
      {
         Serial.println("Failed to open file in writing mode");
      }
      else
      {
         file.write(tmpbuf, tmpbuf_len); // payload (image), payload length
         Serial.print("The picture has been saved in ");
         Serial.print(FILE_PHOTO);
         Serial.print(" - Size: ");
         Serial.print(tmpbuf_len);
         Serial.println(" bytes");
      }
      free(tmpbuf);
      file.close();*/
   return this->frame->buf;
}

/*! returnFrameBuffer()
   @brief return the frame buffer to the camera
   @note call after framebuffer is given to detector input
   @param void
   @return pointer to frame buffer
*/
void AppCamera::returnFrameBuffer()
{
   esp_camera_fb_return(this->frame);
}
/*! switchToJpeg()
   @brief switch camera to high-res jpeg mode, captures image and saves to SPIFFS
   @note
   @param void
   @return void
*/
String AppCamera::switchToJPEG(ipc_warn_t &info)
{
   esp_camera_deinit();
   digitalWrite(PWDN_GPIO_NUM, LOW);
   vTaskDelay(50);
   digitalWrite(PWDN_GPIO_NUM, HIGH);
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
   config.pixel_format = PIXFORMAT_JPEG;
   config.frame_size = FRAMESIZE_SVGA;
   config.jpeg_quality = 15;
   config.fb_count = 1;
   esp_err_t ret = esp_camera_init(&config);
   sensor_t *s = esp_camera_sensor_get();
   s->set_brightness(s, 2);
   s->set_contrast(s, 2);
   s->set_saturation(s, -2);                // -2 to 2
   s->set_special_effect(s, 0);             // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
   s->set_whitebal(s, 1);                   // 0 = disable , 1 = enable
   s->set_awb_gain(s, 1);                   // 0 = disable , 1 = enable
   s->set_wb_mode(s, 0);                    // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
   s->set_exposure_ctrl(s, 1);              // 0 = disable , 1 = enable
   s->set_aec2(s, 0);                       // 0 = disable , 1 = enable
   s->set_ae_level(s, 2);                   // -2 to 2
   s->set_aec_value(s, 400);                // 0 to 1200
   s->set_gain_ctrl(s, 0);                  // 0 = disable , 1 = enable
   s->set_agc_gain(s, 0);                   // 0 to 30
   s->set_gainceiling(s, (gainceiling_t)6); // 0 to 6
   s->set_bpc(s, 1);                        // 0 = disable , 1 = enable
   s->set_wpc(s, 1);                        // 0 = disable , 1 = enable
   s->set_raw_gma(s, 1);                    // 0 = disable , 1 = enable (makes much lighter and noisy)
   s->set_lenc(s, 0);                       // 0 = disable , 1 = enable
   s->set_hmirror(s, 0);                    // 0 = disable , 1 = enable
   s->set_vflip(s, 0);                      // 0 = disable , 1 = enable
   s->set_dcw(s, 0);                        // 0 = disable , 1 = enable
   s->set_colorbar(s, 0);                   // 0 = disable , 1 = enable
   vTaskDelay(150);
   digitalWrite(flash_ctrl, HIGH);
   camera_fb_t *tmpFb = esp_camera_fb_get();
   digitalWrite(flash_ctrl, LOW);

   String filename = "/sentinel/" + (String)this->capture_counter;
   img_info_t img_info;
   img_info.index = this->capture_counter;
   img_info.timestamp = 0;
   img_info.image_err = false;
   img_info.send_err = false;
   img_info.send_done = false;
   img_info.is_deleted = false;
   img_info.path = filename;
   img_info.ipc_info = info;
   File file = SPIFFS.open(filename, FILE_WRITE);
   if (!file)
   {
      Serial.println("Failed to open file in writing mode");
      img_info.image_err = true;
   }
   else
   {
      file.write(tmpFb->buf, tmpFb->len); // payload (image), payload length
      Serial.print("The picture has been saved in ");
      Serial.print(filename);
      Serial.print(" - Size: ");
      Serial.print(tmpFb->len);
      Serial.println(" bytes");

      file.close();
   }

   if (uxQueueSpacesAvailable(firebaseSendPayloadQueue) == 0)
      xQueueReset(firebaseSendPayloadQueue);
   if (xQueueSend(firebaseSendPayloadQueue, (void *)&img_info, 0) != 0)
   {
   }
   this->capture_counter++;
   return filename;
}

void AppCamera::switchToGreyscale()
{
   esp_camera_deinit();
   digitalWrite(PWDN_GPIO_NUM, LOW);
   vTaskDelay(50);
   digitalWrite(PWDN_GPIO_NUM, HIGH);
   this->init();
   vTaskDelay(50);
}
/*! ~AppCamera()
   @brief destructor
   @note
   @param void
   @return void
*/
AppCamera::~AppCamera() {}
