#include "callback_dispatcher.h"


void emberAfInit(void)
{
}


void emberAfTick(void)
{
}


void emberAfStackStatus(EmberStatus status)
{
(void)status;
emberAfPluginPollStackStatusCallback(
status
);
}


void emberAfChildJoin(EmberNodeType nodeType,
                      EmberNodeId nodeId)
{
(void)nodeType;
(void)nodeId;
}


void emberAfRadioNeedsCalibrating(void)
{
}


void emberAfStackIsr(void)
{
emberAfPluginCmsisRtosStackIsr();
}


void emberAfMessageSent(EmberStatus status,
                        EmberOutgoingMessage *message)
{
(void)status;
(void)message;
}


void emberAfMacMessageSent(EmberStatus status,
                           EmberOutgoingMacMessage *message)
{
(void)status;
(void)message;
}


void emberAfIncomingMessage(EmberIncomingMessage *message)
{
(void)message;
}


void emberAfIncomingMacMessage(EmberIncomingMacMessage *message)
{
(void)message;
}


void emberAfIncomingBeacon(EmberPanId panId,
                           EmberMacAddress *source,
                           int8_t rssi,
                           bool permitJoining,
                           uint8_t beaconFieldsLength,
                           uint8_t *beaconFields,
                           uint8_t beaconPayloadLength,
                           uint8_t *beaconPayload)
{
(void)panId;
(void)source;
(void)rssi;
(void)permitJoining;
(void)beaconFieldsLength;
(void)beaconFields;
(void)beaconPayloadLength;
(void)beaconPayload;
}


void emberAfActiveScanComplete(void)
{
}


void emberAfEnergyScanComplete(int8_t mean,
                               int8_t min,
                               int8_t max,
                               uint16_t variance)
{
(void)mean;
(void)min;
(void)max;
(void)variance;
}


void emberAfMarkApplicationBuffers(void)
{
emAfPluginCmsisRtosMarkBuffersCallback();
}


void emberAfFrequencyHoppingStartClientComplete(EmberStatus status)
{
(void)status;
}

//------------------------------------------------------------------------------
// Application callbacks weak definitions

WEAK(void emberAfInitCallback(void))
{
}

WEAK(void emberAfTickCallback(void))
{
}

WEAK(void emberAfStackStatusCallback(EmberStatus status))
{
  (void)status;
}

WEAK(void emberAfIncomingMessageCallback(EmberIncomingMessage *message))
{
  (void)message;
}

WEAK(void emberAfIncomingMacMessageCallback(EmberIncomingMacMessage *message))
{
  (void)message;
}

WEAK(void emberAfMessageSentCallback(EmberStatus status,
                                     EmberOutgoingMessage *message))
{
  (void)status;
  (void)message;
}

WEAK(void emberAfMacMessageSentCallback(EmberStatus status,
                                        EmberOutgoingMacMessage *message))
{
  (void)status;
  (void)message;
}

WEAK(void emberAfChildJoinCallback(EmberNodeType nodeType,
                                   EmberNodeId nodeId))
{
  (void)nodeType;
  (void)nodeId;  
}

WEAK(void emberAfActiveScanCompleteCallback(void))
{
}

WEAK(void emberAfEnergyScanCompleteCallback(int8_t mean,
                                            int8_t min,
                                            int8_t max,
                                            uint16_t variance))
{
  (void)mean;
  (void)min;
  (void)max;
  (void)variance;  
}

WEAK(void emberAfMarkApplicationBuffersCallback(void))
{
}

WEAK(void emberAfIncomingBeaconCallback(EmberPanId panId,
                                        EmberMacAddress *source,
                                        int8_t rssi,
                                        bool permitJoining,
                                        uint8_t beaconFieldsLength,
                                        uint8_t *beaconFields,
                                        uint8_t beaconPayloadLength,
                                        uint8_t *beaconPayload))
{
  (void)panId;
  (void)source;
  (void)rssi;
  (void)permitJoining;
  (void)beaconFieldsLength;
  (void)beaconFields;
  (void)beaconPayloadLength;
  (void)beaconPayload;
}

WEAK(void emberAfFrequencyHoppingStartClientCompleteCallback(EmberStatus status))
{
  (void)status;
}

WEAK(void emberAfRadioNeedsCalibratingCallback(void))
{
  emberCalibrateCurrentChannel();
}

WEAK(bool emberAfStackIdleCallback(uint32_t *idleTimeMs))
{
  (void)idleTimeMs;

  return false;
}