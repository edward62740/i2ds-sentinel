#pragma once

#ifdef ESP32
#include "camera/ESP32Camera.h"
#endif

#include "ImageProcessing/Downscale.h"
#include "ImageProcessing/MotionDetection.h"
<<<<<<< HEAD
#include "ImageProcessing/YUV422Decoder.h"
#include "ImageProcessing/RGB565Decoder.h"
=======
>>>>>>> 6ad028a2c49c97937b8459cd1397009f2f004645

#ifdef FS_H
#include "io/BitmapWriter.h"
#include "io/JpegWriter.h"
#endif