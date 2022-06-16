#ifndef APP_CAMERA_H
#define APP_CAMERA_H
#include "esp_camera.h"

/* This class is used to perform camera capture, image saving to SPIFFS and configuration */
class AppCamera
{
public:
    AppCamera();
    ~AppCamera();
    bool init(pixformat_t pixformat);
    uint8_t *captureGreyscale(bool reinitAsJpeg);
    void captureJpeg(bool reinitAsGreyscale);

private:
    void returnBuffer();
};
#endif // APP_CAMERA_H
