#include <Arduino.h>
#include "app_ipc.h"
#include "app_common.h"
#include "app_camera.h"
#include "app_main.h"
#include "auxiliary/app_ui.h"
// IPC consts
uint8_t IPC_START = 0xAF;
uint8_t IPC_END = 0xAC;
// IPC rx buffer
char ipc_recv_buffer[55];

/*! ipcTask()
   @brief task to handle ipc communication with efr32fg23

   @note

   @param void
*/
void ipcTask(void *pvParameters)
{

    while (1)
    {
        if (Serial1.available()) // receive bytes if avail and parse
        {
            size_t ret = 0;
            ret = Serial1.readBytesUntil(IPC_END, (char *)ipc_recv_buffer, sizeof(ipc_recv_buffer)); // read until end byte
            ipcParser(ipc_recv_buffer, ret);
        }
        vTaskDelay(10);
    }
}

/*! ipcParser()
   @brief parses incoming IPC messages and acts appropriately

   @note

   @param buffer pointer to rx serial buffer
   @param len size of rxed buffer
   @return bool success
*/
bool ipcParser(char *buffer, size_t len)
{
    if (len > 255)
        return false;
    char tmpBuf[len];
    memcpy(tmpBuf, buffer, len * sizeof(char));
    if (tmpBuf[0] != (char)IPC_START) // dump message if incorrect start byte
        return false;
    switch (tmpBuf[1]) // determine message type
    {
    case IPC_ERR:
    {
        APP_LOG_ERR("IPC ERROR");
        if (uxQueueSpacesAvailable(uiQueue) == 0)
            xQueueReset(uiQueue);
        uint8_t val = 255;
        if (xQueueSend(uiQueue, (void *)&val, 0) != 0)
        {
        }
        if (uxQueueSpacesAvailable(ipcQueue) == 0)
            xQueueReset(ipcQueue);
        ipc_warn_t warn;
        warn.src_id = (uint16_t)(tmpBuf[2] << 8 | tmpBuf[3]);
        warn.rssi = tmpBuf[4];
        warn.lqi = tmpBuf[5];
        if (xQueueSend(ipcQueue, (void *)&warn, 0) != 0)
        {
        }


        break;
    }
    case 0:
    {
        break;
    }
    }
    return true;
}

/*! cmpDeviceInfo()
   @brief compares the two param and returns the difference

   @note
   @param s1 pointer to DeviceInfo struct
   @param s2 pointer to DeviceInfo struct
   @return uint16_t to indicate binary pos of changed items in struct
*/
uint16_t cmpDeviceInfo(DeviceInfo *s1, DeviceInfo *s2)
{
    uint16_t ret = 0;
    ret += (s1->hw == s2->hw) ? 1 : 0;
    ret += (s1->state == s2->state) ? 2 : 0;
    ret += (s1->battery_voltage == s2->battery_voltage) ? 4 : 0;
    ret += (s1->node_type == s2->node_type) ? 8 : 0;
    ret += (s1->central_id == s2->central_id) ? 16 : 0;
    ret += (s1->self_id == s2->self_id) ? 32 : 0;
    ret += (s1->endpoint == s2->endpoint) ? 64 : 0;
    ret += (s1->trigd == s2->trigd) ? 128 : 0;
    ret += (s1->rssi == s2->rssi) ? 256 : 0;
    ret += (s1->lqi == s2->lqi) ? 512 : 0;
    return ret;
}