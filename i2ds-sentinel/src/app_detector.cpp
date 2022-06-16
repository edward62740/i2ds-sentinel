#include <Arduino.h>
#include "app_detector.h"
#include "app_camera.h"
#include "app_common.h"
#include <EloquentTinyML.h>
#include <eloquent_tinyml/tensorflow/person_detection.h>

Eloquent::TinyML::TensorFlow::PersonDetection<imageWidth, imageHeight> detector;

AppDetector::AppDetector(detector_ctx_t *ctx)
{
    this->ctx = ctx;
    this->ctx->send = xSemaphoreCreateBinary();
    this->ctx->frame = xQueueCreate(1, sizeof(uint8_t *));
}

AppDetector::~AppDetector()
{
}

void AppDetector::startDetectorTask(TaskHandle_t handle)
{
    xTaskCreatePinnedToCore(this->startDetectorTaskImpl, "detector", 8192, this, 1, &handle, 1);
    this->task = &handle;
}

void AppDetector::startDetectorTaskImpl(void *_this)
{
    static_cast<AppDetector *>(_this)->detectorTask();
}
/*! detectorTask()
   @brief task to run person detection
   @note
   @param void
*/
void AppDetector::detectorTask()
{
    APP_LOG_ERR("IS RUNNING");

    xSemaphoreGive(ctx->send);
    delay(500);

    detector.begin();
    APP_LOG_INFO("SEMAPHORE GIVEN");
    while (1)
    {
        // appCam.captureJpeg();
        if (uxQueueMessagesWaiting(ctx->frame) > 0)
        {
            uint8_t *frameBuf;
            APP_LOG_INFO("MSG WAITING");
            if (xQueueReceive(ctx->frame, &frameBuf, 1) == pdPASS)
            {
                APP_LOG_INFO("MSG RXED");
                bool isPersonInFrame = detector.detectPerson(frameBuf);
                xSemaphoreGive(ctx->send);
                APP_LOG_INFO("SEMAPHORE GIVEN");
                ctx->result.detected = isPersonInFrame;
                ctx->result.posConfidence = detector.getPersonScore();
                ctx->result.negConfidence = detector.getNotPersonScore();
                ctx->result.success = detector.isOk();
            }
        }
        vTaskDelay(5);
    }
}
