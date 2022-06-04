#define CAMERA_MODEL_AI_THINKER
#include <EloquentTinyML.h>
#include <eloquent_tinyml/tensorflow/person_detection.h>
#include "camera.h"
#include "WS2812FX.h"
#include "Adafruit_NeoPixel.h"
#include "Firebase_ESP_Client.h"
#include "WiFi.h"
#include "VL53L1X.h"
#include "LIS2DW12.h"
#include "FS.h"
#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true
const uint16_t imageWidth = 160;
const uint16_t imageHeight = 120;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
Eloquent::TinyML::TensorFlow::PersonDetection<imageWidth, imageHeight> detector;
String devicename = "I2DS SENTINEL";
// Define the Firebase Data object

// Assign the project host and api key (required)
void setup()
{

    Serial.begin(115200);

    initCamera();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname(devicename.c_str()); // define hostname
    while (WiFi.status() != WL_CONNECTED)
    {
        APP_LOG_INFO("[INIT] ERROR CONNECTING TO WIFI");
        WiFi.disconnect();
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        vTaskDelay(3500);
    }
    // Assign the api key
    config.api_key = API_KEY;

    // Firebase configs
    Firebase.reconnectWiFi(true);

    Firebase.signUp(&config, &auth, "", "");
    Firebase.begin(&config, &auth);
    detector.setDetectionAbsoluteThreshold(180);
    detector.begin();

    // abort if an error occurred
    if (!detector.isOk())
    {
        Serial.print("Setup error: ");
        Serial.println(detector.getErrorMessage());

        while (true)
            delay(1000);
    }
    ledcSetup(15, 5000, 8);
    ledcAttachPin(4, 15);
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        ESP.restart();
    }
    else
    {
        delay(500);
        Serial.println("SPIFFS mounted successfully");
    }
}

void loop()
{
getFrameJpeg();
    uint8_t *frame = captureFrame();
    
    bool isPersonInFrame = detector.detectPerson(frame);

    if (!detector.isOk())
    {
        Serial.print("Loop error: ");
        Serial.println(detector.getErrorMessage());

        delay(10000);
        return;
    }

    Serial.println(isPersonInFrame ? "Person detected" : "No person detected");
    isPersonInFrame ? ledcWrite(15, 5) : ledcWrite(15, 0);
    Serial.print("\t > It took ");
    Serial.print(detector.getElapsedTime());
    Serial.println("ms to detect");
    Serial.print("\t > Person score: ");
    Serial.println(detector.getPersonScore());
    Serial.print("\t > Not person score: ");
    Serial.println(detector.getNotPersonScore());
    Serial.println(xPortGetFreeHeapSize());
    if (isPersonInFrame)
    {

        if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */, FILE_PHOTO /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, FILE_PHOTO /* path of remote file stored in the bucket */, "image/jpeg" /* mime type */))
        {
            Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
        }
        else
        {
            Serial.println(fbdo.errorReason());
        }
    }
}