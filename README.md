# I2DS Sentinel

## Overview
This is part of [I<sup>2</sup>DS](https://github.com/edward62740/i2ds). Utilizes tflite-micro person detector model to detect the presence of people, as well as capture photos when requested by other sensors within the network.

<img src="https://github.com/edward62740/i2ds-sentinel/blob/master/Documentation/i2ds-sentinel.jpg" alt="sentinel" width="200"/>

## Operation
This device performs a few main functions:
* Uses person detection to detect presence of humans + _ongoing_ visualize activity over time (https://github.com/edward62740/i2ds-sentinel/issues/7)
* Takes pictures when intrusion is detected by the internal sensor network and uploads to Firebase storage to be viewed through app

## Hardware
* ESP32-CAM board with 4Mb PSRAM (mounted on 2.54mm sockets above mainboard)
* EFR32FG23 microcontroller for receiving warnings from the I<sup>2</sup>DS sensor network
* VL53L1X ToF sensor
* Array of 10 WS2812-2020 RGB LEDs
* LIS2DH12TR tri-axis accelerometer
* USB bridge and power management circuits

<img src="https://github.com/edward62740/i2ds-sentinel/blob/master/Documentation/pcb.jpg" alt="sentinel" width="200"/>
  
## Design Constraints
In this project, the main constraints were on system memory.
* Maximum statically allocated RAM is 160kB
* Large amounts of dynamically allocated memory, i.e SSL/TLS, frame buffer

The issues were resolved by utilizing the PSRAM to store most of the Firebase-related protocol overheads + tf arena.


In addition, the tflite person detection model was found to run too slowly for any useful real-time inference. 
The [ESP-NN](https://github.com/espressif/esp-nn) kernel optimizations were used to bring down the inference time from ca. 5000ms to ca. 400ms, allowing for a practical implementation of this model in the ESP32.

_The build flags in platformio.ini must be used or the code will not compile with the kernel optimizations._
<br>

The OV2640 camera was not performant upon restart to JPEG mode, often lacking brightness and appearing tinted. This appears to be a known issue (https://github.com/espressif/esp32-camera/issues/314), (https://github.com/espressif/esp32-camera/issues/383) related to the camera AWB. <br>
The workaround used was to wait for min. 150ms after restarting the camera in order to get a vaguely usable frame, then to persist in JPEG mode until it is no longer needed (implemented in commit [#b9b3d20](https://github.com/edward62740/i2ds-sentinel/commit/b9b3d20fc2c7c2deef02794aad9a063e34ca1ace)). <br><br>

<br>
Note: Due to limited IO, io12 was used for UART rx. ESP32 must be configured in efuse to disregard io12 as strapping pin for VDD_SDIO regulator, or it will fail to boot.

## Integration with I2DS
This device will work with I2DS from commit [#9158914](https://github.com/edward62740/I2DS/commit/91589148e6d0b51ad65f4bae4d2b9b4c82a5fe6a).







