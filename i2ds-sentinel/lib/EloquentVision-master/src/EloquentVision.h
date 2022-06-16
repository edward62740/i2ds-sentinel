#pragma once

#ifdef ESP32
#include "camera/ESP32Camera.h"
#endif

#include "ImageProcessing/Downscale.h"
#include "ImageProcessing/MotionDetection.h"

#ifdef FS_H
#include "io/BitmapWriter.h"
#include "io/JpegWriter.h"
#endif