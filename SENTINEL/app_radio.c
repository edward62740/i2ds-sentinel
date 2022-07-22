#include PLATFORM_HEADER
#include "stack/include/ember.h"
#include "hal/hal.h"
#include "em_chip.h"
#include "em_iadc.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_burtc.h"
#include "em_acmp.h"
#include "sl_app_common.h"
#include "sl_simple_led_instances.h"
#include "poll.h"
#include "app_process.h"
#include "app_framework_common.h"
#include "app_init.h"
#include "app_log.h"


EmberMessageOptions tx_options = EMBER_OPTIONS_ACK_REQUESTED | EMBER_OPTIONS_SECURITY_ENABLED;

/*!
   @brief start connection to network

   @param void

   @return EmberStatus returns 0 if successful
*/
EmberStatus applicationSensorRadioInit(void)
{
  EmberStatus status;
  EmberNetworkParameters parameters;
  MEMSET(&parameters, 0, sizeof(EmberNetworkParameters));
  parameters.radioTxPower = 140;
  parameters.radioChannel = 11;
  parameters.panId = 0x01FF;
  status = emberJoinNetwork(EMBER_STAR_END_DEVICE, &parameters);
  return status;
}

/*!
   @brief check if radio is busy

   @param void

   @return uint16_t
*/
uint16_t applicationSensorCheckRadioBusy(void){
  return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0 ;
}

/*!
   @brief transmit MSG_INIT for identification upon joining network

   @param void

   @return EmberStatus returns 0 if successful
*/
EmberStatus applicationSensorTxInit(void)
{
  uint8_t buffer[13];
  buffer[0] = 0xFF & (uint8_t) MSG_INIT;
  buffer[1] = 0xFF & (uint8_t) (selfInfo.hw);
  buffer[2] = 0xFF & (uint8_t) (selfInfo.state);
  buffer[3] = 0xFF & (uint8_t) (selfInfo.battery_voltage >> 24);
  buffer[4] = 0xFF & (uint8_t) (selfInfo.battery_voltage >> 16);
  buffer[5] = 0xFF & (uint8_t) (selfInfo.battery_voltage >> 8);
  buffer[6] = 0xFF & (uint8_t) (selfInfo.battery_voltage);
  buffer[7] = 0xFF & (uint8_t) (selfInfo.node_type);
  buffer[8] = 0xFF & (uint8_t) (selfInfo.central_id >> 8);
  buffer[9] = 0xFF & (uint8_t) (selfInfo.central_id);
  buffer[10] = 0xFF & (uint8_t) (selfInfo.self_id >> 8);
  buffer[11] = 0xFF & (uint8_t) (selfInfo.self_id);
  buffer[12] = 0xFF & (uint8_t) (selfInfo.endpoint);
  return emberMessageSend (selfInfo.central_id,
  SENSOR_SINK_ENDPOINT, // endpoint
                    0, // messageTag
                    sizeof(buffer), buffer, tx_options);
}

/*!
   @brief transmit MSG_WARN at the start of sensor trigger

   @param void

   @return EmberStatus returns 0 if successful
*/
EmberStatus applicationSensorTxStartEvent(void)
{
  uint8_t buffer[4];
  buffer[0] = 0xFF & (uint8_t) MSG_WARN;
  buffer[1] = 0xFF & (uint8_t) 0;
  buffer[2] = 0xFF & (uint8_t) 0;
  buffer[3] = 0xFF & (uint8_t) selfInfo.state;
  return emberMessageSend (selfInfo.central_id,
  SENSOR_SINK_ENDPOINT, // endpoint
                    0, // messageTag
                    sizeof(buffer), buffer, tx_options);
}

/*!
   @brief transmit MSG_WARN at the end of sensor trigger

   @param trigd trigger count

   @return EmberStatus returns 0 if successful
*/
EmberStatus applicationSensorTxEndEvent(uint8_t trigd)
{
  uint8_t buffer[4];
  buffer[0] = 0xFF & (uint8_t) MSG_WARN;
  buffer[1] = 0xFF & (uint8_t) 1;
  buffer[2] = 0xFF & (uint8_t) trigd;
  buffer[3] = 0xFF & (uint8_t) selfInfo.state;

  return emberMessageSend (selfInfo.central_id,
  SENSOR_SINK_ENDPOINT, // endpoint
                    0, // messageTag
                    sizeof(buffer), buffer, tx_options);
}

/*!
   @brief transmit MSG_REPORT for updating the coordinator

   @param void

   @return EmberStatus returns 0 if successful
*/
EmberStatus applicationSensorTxRoutine(void)
{
  uint8_t buffer[6];
  buffer[0] = 0xFF & (uint8_t) MSG_REPORT;
  buffer[1] = 0xFF & (uint8_t) (selfInfo.battery_voltage >> 24);
  buffer[2] = 0xFF & (uint8_t) (selfInfo.battery_voltage >> 16);
  buffer[3] = 0xFF & (uint8_t) (selfInfo.battery_voltage >> 8);
  buffer[4] = 0xFF & (uint8_t) (selfInfo.battery_voltage);
  buffer[5] = 0xFF & (uint8_t) 250;
  return emberMessageSend (selfInfo.central_id,
  SENSOR_SINK_ENDPOINT, // endpoint
                    0, // messageTag
                    sizeof(buffer), buffer, tx_options);
}

/*!
   @brief transmit MSG_REPLY in response to MSG_REQUEST commands

   @param void

   @return EmberStatus returns 0 if successful
*/
EmberStatus applicationSensorTxReply(bool success)
{
  uint8_t buffer[3];
  buffer[0] = 0xFF & (uint8_t) MSG_REPLY;
  buffer[1] = 0xFF & (uint8_t) success;
  buffer[2] = 0xFF & (uint8_t) selfInfo.state;
  return emberMessageSend (selfInfo.central_id,
  SENSOR_SINK_ENDPOINT, // endpoint
                    0, // messageTag
                    sizeof(buffer), buffer, tx_options);
}

/*!
   @brief handle received messages and respond accordingly

   @param *message pointer to message structure

   @return true if successful
*/
bool applicationSensorRxMsg(EmberIncomingMessage *message)
{
  bool success = false;
  bool ret = true;
  if ((message->endpoint == selfInfo.endpoint) && (message->source == selfInfo.central_id)
      && (message->options & EMBER_OPTIONS_SECURITY_ENABLED))
    {
      switch (message->payload[0])
        {
        case MSG_WARN:
          {
            ipcWarn((message->payload[2] << 8
                    | message->payload[3]), message->rssi,  message->lqi);
            break;
          }
        case MSG_SYNC:
          {
            applicationSensorTxInit();
            break;
          }
        default:
          ret = false;
          break;
        }
      return ret;
    }
  else return false;

}
