#ifndef APP_CAMERA_H
#define APP_CAMERA_H
#include "esp_camera.h"

/* This class is used to perform camera capture, image saving to SPIFFS and configuration */
class AppCamera
{
public:
    AppCamera();
    ~AppCamera();
    void init();
    uint8_t *captureGreyscale();
    void captureJpeg();
};
#endif // APP_CAMERA_H
