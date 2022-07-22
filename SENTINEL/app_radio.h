#ifndef APP_RADIO_H
#define APP_RADIO_H

EmberStatus applicationSensorRadioInit(void);
EmberStatus applicationSensorTxInit(void);
EmberStatus applicationSensorTxStartEvent(void);
EmberStatus applicationSensorTxEndEvent(uint8_t trigd);
EmberStatus applicationSensorTxRoutine(void);
EmberStatus applicationSensorTxReply(bool success);
bool applicationSensorRxMsg(EmberIncomingMessage *message);

#endif  // APP_RADIO_H
