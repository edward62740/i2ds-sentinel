/***************************************************************************//**
 * @file
 * @brief LED Driver Instances
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "sl_simple_led.h"
#include "em_gpio.h"
#include "sl_simple_led_comms_config.h"
#include "sl_simple_led_stat_config.h"

sl_simple_led_context_t simple_comms_context = {
  .port = SL_SIMPLE_LED_COMMS_PORT,
  .pin = SL_SIMPLE_LED_COMMS_PIN,
  .polarity = SL_SIMPLE_LED_COMMS_POLARITY,
};

const sl_led_t sl_led_comms = {
  .context = &simple_comms_context,
  .init = sl_simple_led_init,
  .turn_on = sl_simple_led_turn_on,
  .turn_off = sl_simple_led_turn_off,
  .toggle = sl_simple_led_toggle,
  .get_state = sl_simple_led_get_state,
};
sl_simple_led_context_t simple_stat_context = {
  .port = SL_SIMPLE_LED_STAT_PORT,
  .pin = SL_SIMPLE_LED_STAT_PIN,
  .polarity = SL_SIMPLE_LED_STAT_POLARITY,
};

const sl_led_t sl_led_stat = {
  .context = &simple_stat_context,
  .init = sl_simple_led_init,
  .turn_on = sl_simple_led_turn_on,
  .turn_off = sl_simple_led_turn_off,
  .toggle = sl_simple_led_toggle,
  .get_state = sl_simple_led_get_state,
};

const sl_led_t *sl_simple_led_array[] = {
  &sl_led_comms,
  &sl_led_stat
};

void sl_simple_led_init_instances(void)
{
  sl_led_init(&sl_led_comms);
  sl_led_init(&sl_led_stat);
}
