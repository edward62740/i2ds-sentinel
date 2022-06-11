#ifndef APP_CAMERA_H
#define APP_CAMERA_H
<<<<<<< HEAD
#include <Arduino.h>

uint8_t *captureFrame();

void getFrameJpeg();

#endif  // APP_CAMERA_H
=======
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
>>>>>>> 6ad028a2c49c97937b8459cd1397009f2f004645
