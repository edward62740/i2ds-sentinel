#ifndef APP_COMMON_H
#define APP_COMMON_H
#include <Arduino.h>

/* GLOBAL CONFIG */
#define FIREBASE_PROJECT_ID "█████████"
#define API_KEY ""
#define FW_VERSION "v1.1"
#define EUSART_IPC_BAUD 115200
#define SPI_DISPLAY_FREQ 48000000
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#ifdef DEBUG
#define DEBUG_LOGS 1
#else
#define DEBUG_LOGS 0
#endif

#define APP_LOG_START()           do { if (DEBUG_LOGS)  Serial.begin(115200); } while (0)
#define APP_LOG_INFO(...) \
            do { if (DEBUG_LOGS)  Serial.println(__VA_ARGS__); } while (0)

// Photo File Name to save in SPIFFS
#define FILE_PHOTO "/data/photo.jpeg"
#define STORAGE_BUCKET_ID "█████████.appspot.com"



#endif  // APP_COMMON_H
