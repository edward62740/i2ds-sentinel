#ifndef APP_DETECTOR_H
#define APP_DETECTOR_H

typedef struct
{
    uint8_t posConfidence;
    uint8_t negConfidence;
    bool detected;
    bool success;
} detector_result_t;

typedef struct
{
    TaskHandle_t detector; // task handle to be given to detector
    QueueHandle_t frame; // queue to send frame pointer to detector
    SemaphoreHandle_t send; // semaphore which acts as clear-to-send indicator
    detector_result_t result; // structure to store result of detector
} detector_ctx_t;


class AppDetector
{
public:
    AppDetector(detector_ctx_t *ctx);
    ~AppDetector();
    void startDetectorTask(TaskHandle_t handle);

private:
    TaskHandle_t *task;
    detector_ctx_t *ctx;
    void detectorTask();
    static void startDetectorTaskImpl(void *_this);
};

#endif // APP_DETECTOR_H