#ifndef APP_IPC_H
#define APP_IPC_H


#include PLATFORM_HEADER
#include "app_process.h"
#include "stack/include/ember.h"
#include "hal/hal.h"
#include "em_chip.h"
#include "app_log.h"
#include "sl_app_common.h"
#include "app_framework_common.h"
#include "sl_simple_led_instances.h"
#include "app_process.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_eusart.h"

#define IPC_RX_MAX_SIZE  30
#define IPC_TX_MAX_SIZE  255
#define IPC_START_BYTE   0xAF
#define IPC_END_BYTE     0xAC

extern volatile uint8_t ipcRxBuffer[IPC_RX_MAX_SIZE];
extern volatile uint8_t ipcTxBuffer[IPC_TX_MAX_SIZE];

extern volatile uint32_t ipcRxPos;
extern volatile uint32_t ipcTxLen;
extern volatile bool ipcDataReady;

typedef enum {     /* IPC message identification byte */
  IPC_CHANGE,          // (C -> EXT) notify change of information
  IPC_LIST,            // (C -> EXT) send list of connected sensors and associated information
  IPC_REQUEST,         // (C <- EXT) request sensor change state
  IPC_REQUEST_ACK,     // (C -> EXT) ack IPC_REQUEST
  IPC_REQUEST_DONE,    // (C -> EXT) finished IPC_REQUEST
  IPC_LIST_CTS,        // (C <- EXT) ack IPC_CHANGE and request IPC_LIST
  IPC_REPORT,
  IPC_ERR,
} ipc_message_pid_t;


void ipcReplyHandler(void);
void ipcWarn(EmberNodeId source, int8_t rssi, uint8_t lqi);
void ipcKeepAlive(void);
void ipcInitThread(void);
void ipcRtosTask(void *p_arg);

void startIPC(void);
#endif  // APP_RADIO_H

