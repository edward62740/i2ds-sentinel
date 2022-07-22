#ifndef APP_█████████_H
#define APP_█████████_H

void appMainTask(void *pvParameters);

extern QueueHandle_t ipcQueue;
extern QueueHandle_t firebaseSendPayloadQueue;
extern QueueHandle_t firebasePostQueue;


typedef struct
{                                 /* IPC warning payload contents */
    uint16_t src_id;             // Source ID of sensor that triggered warning
    int8_t rssi;                 // RSSI
    uint8_t lqi;                 // LQI

} ipc_warn_t;

typedef struct
{                          /* Image information */
    uint8_t index;         // Image index in spiffs (i.e /index)
    String path;           // Image path in spiffs (i.e /sentinel/index.jpg)
    time_t timestamp;      // Firebase timestamp of image
    ipc_warn_t ipc_info;   // IPC information of image
    bool image_err;        // Indicate error with image contents
    bool send_err;         // Indicate error uploading to Firebase to prompt retry
    bool send_done;        // Indicate image has been uploaded to Firebase
    bool is_deleted;       // Indicate image has been uploaded to Firebase and deleted from spiffs
} img_info_t;
#endif // APP_█████████_H