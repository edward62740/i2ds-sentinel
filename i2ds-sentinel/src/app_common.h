#ifndef APP_COMMON_H
#define APP_COMMON_H
#include <Arduino.h>


// Photo File Name to save in SPIFFS
#define FILE_PHOTO "/data/photo.jpeg"
#define STORAGE_BUCKET_ID ""

/* GLOBAL CONFIG */
#define FIREBASE_PROJECT_ID "█████████"
#define DATABASE_URL "" 
#define API_KEY ""
#define FW_VERSION "v1.0"
#define EUSART_IPC_BAUD 921600
#define SPI_DISPLAY_FREQ 48000000
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define DEBUG 1
#ifdef DEBUG
#define DEBUG_LOGS 1
#else
#define DEBUG_LOGS 0
#endif

#define APP_LOG_START()           do { if (DEBUG_LOGS)  Serial.begin(115200); } while (0)
#define APP_LOG_INFO(...) \
            do { if (DEBUG_LOGS)  Serial.print("(" + (String)millis() + ") I: "); Serial.println(__VA_ARGS__); } while (0)
#define APP_LOG_WARN(...) \
            do { if (DEBUG_LOGS)  Serial.print("(" + (String)millis() + ") W: "); Serial.println(__VA_ARGS__); } while (0)
#define APP_LOG_ERR(...) \
            do { if (DEBUG_LOGS)  Serial.print("(" + (String)millis() + ") E: "); Serial.println(__VA_ARGS__); } while (0)

#define IPC_RESPONSE_TIMEOUT_MS 2000 // Sensor poll interval
#define MANAGER_MAX_DEVICE_NOMSG_MS 15000 // Timeout for device 'alive' status
#define FIREBASE_UPDATE_INTERVAL_MS 5000 // Regular firebase update interval
#define FIREBASE_MAX_COMBINED_REQUEST_DURATION 25000 // (Depreciated)
#define FIREBASE_WIFI_RETRY_INTERVAL_MS 3500 // Wi-Fi reconnection interval 
#define FIREBASE_WIFI_MAX_NO_CONNECTION_MS 15000 // Restarts processor if connection is lost for this duration and unable to reconnect

/* TASK CONSTS */
#define FIREBASE_DATA_OBJECT_PAYLOAD_SIZE_BYTES 8192
#define MAX_I2DS_DEVICE_COUNT 30
#define MAX_IPC_REQUEST_RETRY 5
#define MAX_IPC_REQUEST_RESEND 2
#define MAX_FIREBASE_SET_RETRY 3
#define MAX_FIREBASE_ERROR_QUEUE 10
#define MAX_PENDING_DEVICEINFO_QUEUE 10
#define MAX_FIREBASE_REQUEST_QUEUE 10

const uint16_t imageWidth = 160;
const uint16_t imageHeight = 120;
#endif  // APP_COMMON_H
