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
    TaskHandle_t detector;
    QueueHandle_t frame;
    SemaphoreHandle_t send;
    detector_result_t result;
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