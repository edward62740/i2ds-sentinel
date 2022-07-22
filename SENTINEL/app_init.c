#include "app_log.h"
#include "sl_app_common.h"
#include "stack/include/ember.h"
#include "app_process.h"
#include "app_framework_common.h"
#include "sl_simple_led_instances.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_eusart.h"
#include "app_radio.h"

EmberKeyData security_key = { .contents = { █████████
    █████████
    █████████ } };

void emberAfInitCallback(void)
{

  EmberStatus status;
   while (status != EMBER_SUCCESS)
     {
       sl_sleeptimer_delay_millisecond (500);
       status = emberNetworkInit ();
     }

   sl_sleeptimer_delay_millisecond (100);
   app_log_info("Network status 0x%02X\n", status);

   while (emberSetSecurityKey (&security_key) != EMBER_SUCCESS)
     ;
   app_log_info("Network status 0x%02X\n", status);
   status = applicationSensorRadioInit ();
   app_log_info("Network status 0x%02X\n", status);
}
