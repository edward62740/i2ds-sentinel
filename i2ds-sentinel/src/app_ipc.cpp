#include <Arduino.h>
#include "app_ipc.h"
#include "app_common.h"

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
    delay(100);
    while (1)
    {
        if (Serial1.available()) // receive bytes if avail and parse
        {
            size_t ret = 0;
            ret = Serial1.readBytesUntil(IPC_END, (char *)ipc_recv_buffer, sizeof(ipc_recv_buffer)); // read until end byte
            ipcParser(ipc_recv_buffer, ret);
        }
        vPortYield();
        vTaskDelay(125);
        /*log_d("Total heap: %d", ESP.getHeapSize());
        log_d("Free heap: %d", ESP.getFreeHeap());
        log_d("Total PSRAM: %d", ESP.getPsramSize());
        log_d("Free PSRAM: %d", ESP.getFreePsram());
        log_d("Free heap min: %d", ESP.getMinFreeHeap());*/
        vTaskDelay(125);
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
    case IPC_REPORT:
    {
        DeviceInfo tmpInfo;
        tmpInfo.self_id = (uint16_t)((tmpBuf[2] << 8) | tmpBuf[3]);
        tmpInfo.battery_voltage = (uint32_t)((tmpBuf[4] << 24) | (tmpBuf[5] << 16) | (tmpBuf[6] << 8) | tmpBuf[7]);
        tmpInfo.state = (uint8_t)tmpBuf[8];
        tmpInfo.rssi = (int8_t)tmpBuf[9];
        tmpInfo.lqi = (uint8_t)tmpBuf[10];

        break;
    }

    // message upon change of device status
    case IPC_CHANGE:
    {
        uint16_t id = (tmpBuf[2] << 8) | tmpBuf[3];
        uint8_t state = tmpBuf[4];
        uint8_t count = tmpBuf[6];
        uint8_t tmpIndex = 0;

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