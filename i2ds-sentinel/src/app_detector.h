#ifndef APP_DETECTOR_H
#define APP_DETECTOR_H

typedef struct
{
    uint8_t posConfidence; // person detected score (0-100)
    uint8_t negConfidence; // not person detected score (0-100)
    bool detected;         // person detected boolean (i.e posConfidence > negConfidence && posConfidence > 67)
    bool success;          // indicates valid detection results
} detector_result_t;

typedef struct
{
    TaskHandle_t detector;    // task handle to be given to detector
    QueueHandle_t frame;      // queue to send frame pointer to detector
    SemaphoreHandle_t send;   // semaphore which acts as ready indicator
    detector_result_t result; // structure to store result of detector
} detector_ctx_t;             // context to be passed to detector

class AppDetector
{
public:
    AppDetector(detector_ctx_t *ctx);
    ~AppDetector();
    void startDetectorTask(TaskHandle_t handle, UBaseType_t priority, BaseType_t coreId);

private:
    TaskHandle_t *task;
    detector_ctx_t *ctx;
    void detectorTask();
    static void startDetectorTaskImpl(void *_this);
};

#endif // APP_DETECTOR_H