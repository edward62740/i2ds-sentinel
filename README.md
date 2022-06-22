# I2DS Sentinel

## Overview
This is part of [I<sup>2</sup>DS](https://github.com/edward62740/i2ds). Utilizes tflite-micro person detector model to detect the presence of people, as well as capture photos when requested by other sensors within the network.

## Operation
## Hardware
* ESP32-CAM board with 4Mb (minimum) PSRAM
* EFR32FG23 microcontroller for receiving warnings from the I<sup>2</sup>DS sensor network
* VL53L1X ToF sensor
* Array of 10 WS2812-2020 RGB LEDs
* LIS2DH12TR tri-axis accelerometer
* USB bridge and power management circuits

## Main Design Considerations
In this project, the main constraints were on system memory.
* Maximum statically allocated RAM is 160kB
* Large amounts of dynamically allocated memory, i.e SSL/TLS, frame buffer

In addition, the tflite person detection model was found to run too slowly for any useful real-time inference. The [ESP-NN](https://github.com/espressif/esp-nn) kernel optimizations were used to bring down the inference time from ca. 5000ms to ca. 400ms, allowing for a practical implementation of this model in the ESP32.



