#ifndef APP_UI_H
#define APP_UI_H

void ledTask (void *pvParameters);
extern QueueHandle_t uiQueue;

#endif // APP_UI_H