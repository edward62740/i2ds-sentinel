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
#include "app_ipc.h"
#include "app_radio.h"

volatile uint8_t ipcTxBuffer[IPC_TX_MAX_SIZE];
volatile uint32_t ipcRxPos = 0;
volatile uint32_t ipcTxLen = 0;
volatile bool ipcDataReady = false;
volatile bool ipcRespReady = false;

osThreadId_t ipcId;


void EUSART1_TX_IRQHandler(void)
{
  EUSART_IntClear (EUSART1, EUSART_IF_TXFL);
  if(ipcRespReady){
      ipcRespReady = false;
      EUSART1->TXDATA = IPC_START_BYTE;
      while (!(EUSART1->STATUS & (1 << 6)))
        ;
      for (uint32_t i = 0; i < ipcTxLen; i++)
        {
          EUSART1->TXDATA = ipcTxBuffer[i];
          while (!(EUSART1->STATUS & (1 << 6)))
            ;
        }
      EUSART1->TXDATA = IPC_END_BYTE;
      while (!(EUSART1->STATUS & (1 << 6)))
        ;
      EUSART_IntDisable (EUSART1, EUSART_IEN_TXFL);
  }
}



void ipcWarn(EmberNodeId source, int8_t rssi, uint8_t lqi)
{
  uint8_t tmpTxIndex = 0;
  ipcTxBuffer[tmpTxIndex++] = 0xFF & (uint8_t) IPC_ERR;
  ipcTxBuffer[tmpTxIndex++] = 0xFF & (uint8_t) (source >> 8);
  ipcTxBuffer[tmpTxIndex++] = 0xFF & (uint8_t) source;
  ipcTxBuffer[tmpTxIndex++] = 0xFF & (int8_t) rssi;
  ipcTxBuffer[tmpTxIndex++] = 0xFF & (uint8_t) lqi;
  ipcTxBuffer[tmpTxIndex] = (uint8_t) tmpTxIndex;
  ipcTxLen = tmpTxIndex + 1;
  ipcRespReady = true;
  EUSART_IntEnable (EUSART1, EUSART_IEN_TXFL);
}

void ipcKeepAlive(void)
{
  uint8_t tmpTxIndex = 0;
  ipcTxBuffer[tmpTxIndex] = (uint8_t) tmpTxIndex;
  ipcTxLen = tmpTxIndex + 1;
  ipcRespReady = true;
  EUSART_IntEnable (EUSART1, EUSART_IEN_TXFL);
}

void ipcInitThread(void)
{
  osThreadAttr_t ipcStackAttribute = {
      "Inter Processor Communication Task",
      osThreadDetached,
      NULL,
      0,
      NULL,
      (1000 * sizeof(void *)) & 0xFFFFFFF8u,
      37,
      0,
      0
    };

    ipcId = osThreadNew(ipcRtosTask,
                                 NULL,
                                 &ipcStackAttribute);
    assert(ipcId != 0);
}

void ipcRtosTask(void *p_arg)
{
  (void)p_arg;

  startIPC();
  ipcRxPos = 0;
  while (true) {
      osDelay(500);
      ipcKeepAlive();
      }

}


void startIPC(void)
{
  CMU_ClockEnable (cmuClock_GPIO, true);
  CMU_ClockEnable (cmuClock_EUSART1, true);
  GPIO_PinModeSet (gpioPortA, 6, gpioModePushPull, 1);

  EUSART_UartInit_TypeDef init = EUSART_UART_INIT_DEFAULT_HF;
  init.baudrate = 921600;
  init.parity = EUSART_FRAMECFG_PARITY_ODD;

  // Route EUSART1 TX and RX to the board controller TX and RX pins
  GPIO->EUSARTROUTE[1].TXROUTE = (gpioPortA << _GPIO_EUSART_TXROUTE_PORT_SHIFT)
      | (6 << _GPIO_EUSART_TXROUTE_PIN_SHIFT);

  // Enable RX and TX signals now that they have been routed
  GPIO->EUSARTROUTE[1].ROUTEEN =GPIO_EUSART_ROUTEEN_TXPEN;

  // Configure and enable EUSART1 for high-frequency (EM0/1) operation
  EUSART_UartInitHf (EUSART1, &init);
 // EUSART_IntEnable (EUSART1, EUSART_IEN_RXFL);
  EUSART_IntDisable (EUSART1, EUSART_IEN_TXFL);

  // Enable NVIC USART sources
 // NVIC_ClearPendingIRQ (EUSART1_RX_IRQn);
 // NVIC_EnableIRQ (EUSART1_RX_IRQn);
  NVIC_ClearPendingIRQ (EUSART1_TX_IRQn);
  NVIC_EnableIRQ (EUSART1_TX_IRQn);
}
