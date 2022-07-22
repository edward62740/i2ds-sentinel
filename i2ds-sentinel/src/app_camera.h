#ifndef APP_CAMERA_H
#define APP_CAMERA_H
#include "esp_camera.h"
#include "app_main.h"
/* This class is used to perform camera capture, image saving to SPIFFS and configuration */
class AppCamera
{
public:
    AppCamera(uint8_t flash_ctrl);
    ~AppCamera();

    esp_err_t init();
    uint8_t *captureGreyscale();
    void returnFrameBuffer();
    String switchToJPEG(ipc_warn_t &info);
    void switchToGreyscale();

private:
    camera_fb_t *frame;
    uint8_t flash_ctrl;
    uint32_t capture_counter;
};
#endif // APP_CAMERA_H
