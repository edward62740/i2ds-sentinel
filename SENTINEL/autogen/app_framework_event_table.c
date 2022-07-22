#include "stack/include/ember-types.h"


extern EmberEventControl emberAfPluginPollEventControl;
void emberAfPluginPollEventHandler(void);
void(*emAppEventsHandlerPtrTable[6])(void) = { NULL, NULL , NULL, NULL, NULL, NULL };

EmberEventControl allocateEvent0 = { EMBER_EVENT_INACTIVE, 0, 0 };
void allocateEvent0Handler(void)
{
  if (emAppEventsHandlerPtrTable[0]) {
    emAppEventsHandlerPtrTable[0]();
  }
}
EmberEventControl allocateEvent1 = { EMBER_EVENT_INACTIVE, 0, 0 };
void allocateEvent1Handler(void)
{
  if (emAppEventsHandlerPtrTable[1]) {
    emAppEventsHandlerPtrTable[1]();
  }
}
EmberEventControl allocateEvent2 = { EMBER_EVENT_INACTIVE, 0, 0 };
void allocateEvent2Handler(void)
{
  if (emAppEventsHandlerPtrTable[2]) {
    emAppEventsHandlerPtrTable[2]();
  }
}
EmberEventControl allocateEvent3 = { EMBER_EVENT_INACTIVE, 0, 0 };
void allocateEvent3Handler(void)
{
  if (emAppEventsHandlerPtrTable[3]) {
    emAppEventsHandlerPtrTable[3]();
  }
}
EmberEventControl allocateEvent4 = { EMBER_EVENT_INACTIVE, 0, 0 };
void allocateEvent4Handler(void)
{
  if (emAppEventsHandlerPtrTable[4]) {
    emAppEventsHandlerPtrTable[4]();
  }
}
EmberEventControl allocateEvent5 = { EMBER_EVENT_INACTIVE, 0, 0 };
void allocateEvent5Handler(void)
{
  if (emAppEventsHandlerPtrTable[5]) {
    emAppEventsHandlerPtrTable[5]();
  }
}

const EmberEventData emAppEvents[] = {
{ &emberAfPluginPollEventControl, emberAfPluginPollEventHandler },
{ &allocateEvent0, allocateEvent0Handler },
{ &allocateEvent1, allocateEvent1Handler },
{ &allocateEvent2, allocateEvent2Handler },
{ &allocateEvent3, allocateEvent3Handler },
{ &allocateEvent4, allocateEvent4Handler },
{ &allocateEvent5, allocateEvent5Handler },
{ NULL, NULL }
};

const uint8_t emAfEventTableOffset = 1;
uint8_t emAfEventTableHandleIndex = 0;
