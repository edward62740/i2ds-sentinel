#include PLATFORM_HEADER
#include "stack/include/ember.h"
#include "hal/hal.h"
#include "em_chip.h"
#include "app_log.h"
#include "poll.h"
#include "sl_app_common.h"
#include "app_process.h"
#include "app_framework_common.h"
#include "app_radio.h"
#include "app_init.h"
#include "sl_simple_led_instances.h"

DeviceInfo selfInfo;
EmberEventControl *report_control;
uint16_t sensor_report_period_ms =  (5 * MILLISECOND_TICKS_PER_SECOND);

/*!       report_handler :: HANDLER
   @brief tx routine every sensor_report_period_ms

   @param void

   @return void
*/
void report_handler(void)
{
  if (!emberStackIsUp()) {
    emberEventControlSetInactive(*report_control);
  } else {
      sl_led_turn_on (&sl_led_stat);
      applicationSensorTxRoutine();
      emberEventControlSetDelayMS(*report_control, sensor_report_period_ms);
  }
  app_log_info("Sent..");
}

bool emberAfCommonOkToEnterLowPowerCallback(bool enter_em2, uint32_t duration_ms)
{
  (void) enter_em2;
  (void) duration_ms;
  return true;
}

void emberAfIncomingMessageCallback (EmberIncomingMessage *message)
{
  sl_led_turn_on (&sl_led_stat);
  app_log_info("recv");
  applicationSensorRxMsg(&(*message));
}

void emberAfMessageSentCallback(EmberStatus status,
                                EmberOutgoingMessage *message)
{
  (void) message;
  if (status != EMBER_SUCCESS) {
  }
  sl_led_turn_off(&sl_led_comms);
  sl_led_turn_off(&sl_led_stat);
}


void emberAfStackStatusCallback(EmberStatus status)
{
  switch (status)
    {
    case EMBER_NETWORK_UP:
      emberAfAllocateEvent (&report_control, &report_handler);
      initSensorInfo (&selfInfo, 139, 0xFA, 0, emberGetNodeType(),
      EMBER_COORDINATOR_ADDRESS,
                      emberGetNodeId(), SENSOR_SINK_ENDPOINT, 0);
     emberEventControlSetDelayMS(*report_control, sensor_report_period_ms);
      sl_led_turn_on (&sl_led_stat);
      applicationSensorTxInit ();
      sl_led_turn_on (&sl_led_comms);
      break;
    case EMBER_NETWORK_DOWN:
      {
        sl_led_toggle (&sl_led_comms);
        sl_led_toggle (&sl_led_stat);
        status = applicationSensorRadioInit ();
      }
      break;
    case EMBER_JOIN_SCAN_FAILED:
      break;
    case EMBER_JOIN_DENIED:
      break;
    case EMBER_JOIN_TIMEOUT:

      {
        sl_led_toggle (&sl_led_comms);
        sl_led_toggle (&sl_led_stat);
        status = applicationSensorRadioInit ();
      }
      break;
    default:
      {
        sl_led_toggle (&sl_led_comms);
        sl_led_toggle (&sl_led_stat);
        status = applicationSensorRadioInit ();
      }
      break;
    }
}

void emberAfTickCallback(void)
{
}

/*!       initSensorInfo :: FUNCTION
   @brief setter for DeviceInfo

   @param *info and its input params

   @return void
*/
void initSensorInfo(volatile DeviceInfo *info, device_hw_t hw, sensor_state_t state, uint32_t battery_voltage, EmberNodeType node_type,
                    EmberNodeId central_id, EmberNodeId self_id, uint8_t endpoint, uint8_t trigd) {
  info->hw = hw;
  info->state = state;
  info->battery_voltage = battery_voltage;
  info->node_type = node_type;
  info->central_id = central_id;
  info->self_id = self_id;
  info->endpoint = endpoint;
  info->trigd = trigd;
}

