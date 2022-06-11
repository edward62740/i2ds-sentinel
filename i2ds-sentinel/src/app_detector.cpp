<<<<<<< HEAD
=======
#include <Arduino.h>
>>>>>>> 6ad028a2c49c97937b8459cd1397009f2f004645
#include "app_detector.h"
#include "app_camera.h"
#include "app_common.h"
#include <EloquentTinyML.h>
#include <eloquent_tinyml/tensorflow/person_detection.h>
#include "FS.h"
#include "SPIFFS.h"
#include "Firebase_ESP_Client.h"
#include "WiFi.h"
<<<<<<< HEAD
=======
#include "esp32-hal-psram.h"
>>>>>>> 6ad028a2c49c97937b8459cd1397009f2f004645
#define FORMAT_SPIFFS_IF_FAILED true

Eloquent::TinyML::TensorFlow::PersonDetection<imageWidth, imageHeight> detector;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
<<<<<<< HEAD

=======
AppCamera appCam;
>>>>>>> 6ad028a2c49c97937b8459cd1397009f2f004645
String devicename = "I2DS SENTINEL";

/*! detectorTask()
   @brief task to run person detection
<<<<<<< HEAD


   @note

=======
   @note
>>>>>>> 6ad028a2c49c97937b8459cd1397009f2f004645
   @param void
*/
void detectorTask(void *pvParameters)
{
<<<<<<< HEAD
     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
=======

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
>>>>>>> 6ad028a2c49c97937b8459cd1397009f2f004645
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
<<<<<<< HEAD


    while (1)
    {
       // getFrameJpeg();
        uint8_t *frame = captureFrame();
        bool isPersonInFrame =true;

=======
    detector.begin();
    SPIFFS.begin();
    pinMode(4, OUTPUT);
    appCam.init();
    delay(1500);
    while (1)
    {
        appCam.captureJpeg();
        delay(500);
        bool isPersonInFrame = detector.detectPerson(appCam.captureGreyscale());
>>>>>>> 6ad028a2c49c97937b8459cd1397009f2f004645
        if (!detector.isOk())
        {
            APP_LOG_ERR("[DETECTOR] ERROR DETECTING PERSON");
            APP_LOG_ERR("[DETECTOR] " + (String)detector.getErrorMessage());
        }
        APP_LOG_INFO(isPersonInFrame ? "[DETECTOR] PERSON DETECTED" : "[DETECTOR] NO PERSON DETECTED");
        APP_LOG_INFO("[DETECTOR] Person detected score: " + (String)detector.getPersonScore());
        APP_LOG_INFO("[DETECTOR] No person detected score: " + (String)detector.getNotPersonScore());
<<<<<<< HEAD
          if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */, FILE_PHOTO /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, FILE_PHOTO /* path of remote file stored in the bucket */, "image/jpeg" /* mime type */))
=======
        if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID /* Firebase Storage bucket id */, FILE_PHOTO /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, FILE_PHOTO /* path of remote file stored in the bucket */, "image/jpeg" /* mime type */))
>>>>>>> 6ad028a2c49c97937b8459cd1397009f2f004645
        {
            Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
        }
        else
        {
            Serial.println(fbdo.errorReason());
        }
<<<<<<< HEAD
       
    }
}

/*! runDetector()
   @brief calls camera to capture frame, runs person detection and returns result, SPIFFS location

   @note
   @param isTriggered bool indicate if the function is being run due to ipc trigger or due to a timer
   @param path String path to SPIFFS location
   @param confidence float confidence of person detection
   @param bool indicating if person is in frame
   @return bool success
*/

=======
        SPIFFS.remove(FILE_PHOTO);
    }
}
>>>>>>> 6ad028a2c49c97937b8459cd1397009f2f004645
