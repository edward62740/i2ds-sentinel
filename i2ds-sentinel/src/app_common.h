#ifndef APP_COMMON_H
#define APP_COMMON_H
#include <Arduino.h>

const int IPC_TX = 2;
const int IPC_RX = 12;
const int I2C_SDA = 15;
const int I2C_SCL = 14;
const int IR_LED = 4;
const int ALARM = 13;
const int LED_ADDR = 16;
const int PWDN_GPIO_NUM = 32;
const int RESET_GPIO_NUM = -1;
const int XCLK_GPIO_NUM = 0;
const int SIOD_GPIO_NUM = 26;
const int SIOC_GPIO_NUM = 27;
const int Y9_GPIO_NUM = 35;
const int Y8_GPIO_NUM = 34;
const int Y7_GPIO_NUM = 39;
const int Y6_GPIO_NUM = 36;
const int Y5_GPIO_NUM = 21;
const int Y4_GPIO_NUM = 19;
const int Y3_GPIO_NUM = 18;
const int Y2_GPIO_NUM = 5;
const int VSYNC_GPIO_NUM = 25;
const int HREF_GPIO_NUM = 23;
const int PCLK_GPIO_NUM = 22;
// Photo File Name to save in SPIFFS
#define STORAGE_BUCKET_ID "█████████.appspot.com"

/* GLOBAL CONFIG */
#define FIREBASE_PROJECT_ID "█████████"
#define DATABASE_URL "█████████"
#define API_KEY "█████████"
#define FW_VERSION "v1.0"
#define EUSART_IPC_BAUD 921600
#define WIFI_SSID "█████████"
#define WIFI_PASSWORD "█████████"
#define DEBUG 1
#ifdef DEBUG
#define DEBUG_LOGS 1
#else
#define DEBUG_LOGS 0
#endif

#define APP_LOG_START()           \
    do                            \
    {                             \
        if (DEBUG_LOGS)           \
            Serial.begin(115200); \
    } while (0)
#define APP_LOG_INFO(...)                                   \
    do                                                      \
    {                                                       \
        if (DEBUG_LOGS)                                     \
            Serial.print("(" + (String)millis() + ") I: "); \
        Serial.println(__VA_ARGS__);                        \
    } while (0)
#define APP_LOG_WARN(...)                                   \
    do                                                      \
    {                                                       \
        if (DEBUG_LOGS)                                     \
            Serial.print("(" + (String)millis() + ") W: "); \
        Serial.println(__VA_ARGS__);                        \
    } while (0)
#define APP_LOG_ERR(...)                                    \
    do                                                      \
    {                                                       \
        if (DEBUG_LOGS)                                     \
            Serial.print("(" + (String)millis() + ") E: "); \
        Serial.println(__VA_ARGS__);                        \
    } while (0)

#define IPC_RESPONSE_TIMEOUT_MS 2000                 // Sensor poll interval
#define MANAGER_MAX_DEVICE_NOMSG_MS 15000            // Timeout for device 'alive' status
#define FIREBASE_UPDATE_INTERVAL_MS 5000             // Regular firebase update interval
#define FIREBASE_MAX_COMBINED_REQUEST_DURATION 25000 // (Depreciated)
#define FIREBASE_WIFI_RETRY_INTERVAL_MS 3500         // Wi-Fi reconnection interval
#define FIREBASE_WIFI_MAX_NO_CONNECTION_MS 15000     // Restarts processor if connection is lost for this duration and unable to reconnect

/* TASK CONSTS */
#define FIREBASE_DATA_OBJECT_PAYLOAD_SIZE_BYTES 8192
#define MAX_I2DS_DEVICE_COUNT 30
#define MAX_IPC_REQUEST_RETRY 5
#define MAX_IPC_REQUEST_RESEND 2
#define MAX_FIREBASE_SET_RETRY 3
#define MAX_FIREBASE_ERROR_QUEUE 10
#define MAX_PENDING_DEVICEINFO_QUEUE 10
#define MAX_FIREBASE_REQUEST_QUEUE 10


#define DETECTOR_POS_THRESHOLD 67
const uint16_t imageWidth = 96;
const uint16_t imageHeight = 96;
#endif // APP_COMMON_H
