#include "stack/include/ember.h"


// Init
void emberAfInit(void);

// Tick
void emberAfTick(void);

// Stack Status
void emberAfStackStatus(EmberStatus status);
void emberAfPluginPollStackStatusCallback(
EmberStatus status
);

// Child Join
void emberAfChildJoin(EmberNodeType nodeType,
                      EmberNodeId nodeId);

// Radio Needs Calibrating
void emberAfRadioNeedsCalibrating(void);

// Stack ISR
void emberAfStackIsr(void);
void emberAfPluginCmsisRtosStackIsr(void);

// Message Sent
void emberAfMessageSent(EmberStatus status,
                        EmberOutgoingMessage *message);

// Mac Message Sent
void emberAfMacMessageSent(EmberStatus status,
                           EmberOutgoingMacMessage *message);

// Incoming Message
void emberAfIncomingMessage(EmberIncomingMessage *message);

// Mac Incoming Message
void emberAfIncomingMacMessage(EmberIncomingMacMessage *message);

// Incoming Beacon
void emberAfIncomingBeacon(EmberPanId panId,
                           EmberMacAddress *source,
                           int8_t rssi,
                           bool permitJoining,
                           uint8_t beaconFieldsLength,
                           uint8_t *beaconFields,
                           uint8_t beaconPayloadLength,
                           uint8_t *beaconPayload);

// Active Scan Complete
void emberAfActiveScanComplete(void);

// Energy Scan Complete
void emberAfEnergyScanComplete(int8_t mean,
                               int8_t min,
                               int8_t max,
                               uint16_t variance);

// Mark Application Buffers
void emberAfMarkApplicationBuffers(void);
void emAfPluginCmsisRtosMarkBuffersCallback(void);

// Frequency Hopping Start Client Complete
void emberAfFrequencyHoppingStartClientComplete(EmberStatus status);
