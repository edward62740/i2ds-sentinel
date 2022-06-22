#include <Arduino.h>
#include "app_detector.h"
#include "app_camera.h"
#include "app_common.h"

#include "person_detect_model_data.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
tflite::ErrorReporter *error_reporter = nullptr;
const tflite::Model *model = nullptr;
tflite::MicroInterpreter *interpreter = nullptr;
TfLiteTensor *input = nullptr;

// In order to use optimized tensorflow lite kernels, a signed int8_t quantized
// model is preferred over the legacy unsigned model format. This means that
// throughout this project, input images must be converted from unisgned to
// signed format. The easiest and quickest way to convert from unsigned to
// signed 8-bit integers is to subtract 128 from the unsigned value to get a
// signed value.

#ifdef CONFIG_IDF_TARGET_ESP32S3
constexpr int scratchBufSize = 39 * 1024;
#else
constexpr int scratchBufSize = 0;
#endif
// An area of memory to use for input, output, and intermediate arrays.
constexpr int kTensorArenaSize = 81 * 1024 + scratchBufSize;
static uint8_t *tensor_arena; //[kTensorArenaSize]; // Maybe we should move this to external
/*! AppDetector()
   @brief constructor
   @note
   @param *ctx pointer to detector context
*/
AppDetector::AppDetector(detector_ctx_t *ctx)
{
    this->ctx = ctx;
    this->ctx->send = xSemaphoreCreateBinary();
    this->ctx->frame = xQueueCreate(1, sizeof(uint8_t *));
}

/*! ~AppDetector()
   @brief destructor
   @note
   @param void
*/
AppDetector::~AppDetector() {}

/*! startDetectorTask()
   @brief called from outside the class to start the detector task
   @note
   @param handle task handle to be given to detector
*/
void AppDetector::startDetectorTask(TaskHandle_t handle)
{
    xTaskCreatePinnedToCore(this->startDetectorTaskImpl, "detector", 8192, this, 1, &handle, 1);
    this->task = &handle;
}

/*! startDetectorTaskImpl()
   @brief internally called task to run the detector
   @note
   @param *_this pointer to this class
*/
void AppDetector::startDetectorTaskImpl(void *_this)
{
    static_cast<AppDetector *>(_this)->detectorTask();
}

/*! detectorTask()
   @brief task to run person detection when frame pointer is sent over ctx frame queue
   @note
   @param void
*/
void AppDetector::detectorTask()
{
    xSemaphoreGive(ctx->send);
    // Set up logging. Google style is to avoid globals or statics because of
    // lifetime uncertainty, but since this has a trivial destructor it's okay.
    // NOLINTNEXTLINE(runtime-global-variables)
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    // Map the model into a usable data structure. This doesn't involve any
    // copying or parsing, it's a very lightweight operation.
    model = tflite::GetModel(g_person_detect_model_data);
    if (model->version() != TFLITE_SCHEMA_VERSION)
    {
    }

    if (tensor_arena == NULL)
    {
        tensor_arena = (uint8_t *)heap_caps_malloc(kTensorArenaSize, MALLOC_CAP_█████████ | MALLOC_CAP_8BIT);
    }
    if (tensor_arena == NULL)
    {
        printf("Couldn't allocate memory of %d bytes\n", kTensorArenaSize);
        return;
    }
    // Pull in only the operation implementations we need.
    // This relies on a complete list of all the ops needed by this graph.
    // An easier approach is to just use the AllOpsResolver, but this will
    // incur some penalty in code space for op implementations that are not
    // needed by this graph.
    //
    // tflite::AllOpsResolver resolver;
    // NOLINTNEXTLINE(runtime-global-variables)
    static tflite::MicroMutableOpResolver<5> micro_op_resolver;
    micro_op_resolver.AddAveragePool2D();
    micro_op_resolver.AddConv2D();
    micro_op_resolver.AddDepthwiseConv2D();
    micro_op_resolver.AddReshape();
    micro_op_resolver.AddSoftmax();
   

    // Build an interpreter to run the model with.
    // NOLINTNEXTLINE(runtime-global-variables)
    static tflite::MicroInterpreter static_interpreter(
        model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk)
    {
        TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
        return;
    }

    // Get information about the memory area to use for the model's input.
    input = interpreter->input(0);

    while (1)
    {
        if (uxQueueMessagesWaiting(ctx->frame) > 0)
        {
            uint8_t *frameBuf;
            if (xQueueReceive(ctx->frame, &frameBuf, 1) == pdPASS)
            {
                for (int i = 0; i < imageWidth * imageHeight; i++)
                {
                    input->data.int8[i] = ((uint8_t *)frameBuf)[i] ^ 0x80;
                }
                // Run the model on this input and make sure it succeeds.
                if (kTfLiteOk != interpreter->Invoke())
                {
                    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed.");
                }

                TfLiteTensor *output = interpreter->output(0);
                constexpr int kPersonIndex = 1;
                constexpr int kNotAPersonIndex = 0;
                // Process the inference results.
  int8_t person_score = output->data.uint8[kPersonIndex];
  int8_t no_person_score = output->data.uint8[kNotAPersonIndex];
                float person_score_f =
                    (person_score - output->params.zero_point) * output->params.scale;
                float no_person_score_f =
                    (no_person_score - output->params.zero_point) * output->params.scale;
                    Serial.println(person_score_f);
                ctx->result.posConfidence = person_score_f*100;
                ctx->result.negConfidence = no_person_score_f*100;
                vTaskDelay(1); // to avoid watchdog trigger
                xSemaphoreGive(ctx->send);
            }
        }
        vTaskDelay(5);
    }
}
