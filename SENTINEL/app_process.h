#ifndef APP_PROCESS_H
#define APP_PROCESS_H

#include PLATFORM_HEADER
#include "stack/include/ember.h"
#include "hal/hal.h"
#include "em_chip.h"
#include "app_log.h"
#include "poll.h"
#include "em_iadc.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_burtc.h"
#include "em_acmp.h"
#include "sl_app_common.h"
#include "app_process.h"
#include "app_framework_common.h"
#include "app_radio.h"
#include "sl_simple_led_instances.h"

#define MAX_TX_FAILURES     (10u)


typedef enum {     /* Packet identifier byte */
  MSG_INIT,            // (S -> C) notify sensor type and features
  MSG_REPORT,          // (S -> C) report battery levels and status
  MSG_WARN = 0x9A,     // (S -> C) report triggered sensor
  MSG_REQUEST,         // (S <- C) request change status
  MSG_REPLY,           // (S <- C) ack REQUEST
  MSG_SYNC = 0xFF,     // (S <- C) request INIT
} message_pid_t;

typedef enum {     /* Sensor state byte */
  S_ACTIVE = 0x05,   // Sensor element active
  S_INACTIVE,        // Sensor element inactive
  S_FAULT_HW = 0xCA, // Hardware fault detected
  S_FAULT_OPN,       // Operational fault detected
  S_ALERTING,        // Sensor element triggering
  S_COLDSTART = 0xE0,
  S_CALIBRATING,
} sensor_state_t;

typedef enum {     /* Hardware identification byte */
  HW_CPN = 0x88,      // Control Panel Node (Coordinator)
  HW_PIRSN,           // PIR Sensor Node (Sensor)
  HW_ACSN,            // Access Control Sensor Node (Sensor)
} device_hw_t ;

typedef enum {     /* Request identifier byte */
  REQ_STATE,       // Request state change
  REQ_TXPWR,       // Request change of radio transmit power in positive deci-dbm
  REQ_REPORT,      // Request REPORT packet
  REQ_LED,         // Request enable/disable LEDs
} message_request_t;

typedef struct {   /* Sensor info */
  device_hw_t hw;
  sensor_state_t state;
  uint32_t battery_voltage;
  EmberNodeType node_type;
  EmberNodeId central_id;
  EmberNodeId self_id;
  uint8_t endpoint;
  uint8_t trigd;
} DeviceInfo;
extern DeviceInfo selfInfo;


void report_handler(void);
void initSensorInfo(volatile DeviceInfo *info, device_hw_t hw, sensor_state_t state, uint32_t battery_voltage, EmberNodeType node_type,
                    EmberNodeId central_id, EmberNodeId self_id, uint8_t endpoint, uint8_t trigd);

#endif  // APP_PROCESS_H
