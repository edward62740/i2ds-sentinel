#include "sl_event_handler.h"

#include "em_chip.h"
#include "sl_device_init_nvic.h"
#include "sl_device_init_dcdc.h"
#include "sl_hfxo_manager.h"
#include "sl_device_init_hfxo.h"
#include "sl_device_init_clocks.h"
#include "sl_device_init_emu.h"
#include "pa_conversions_efr32.h"
#include "sl_sleeptimer.h"
#include "app_log.h"
#include "cmsis-rtos-support.h"
#include "sl_iostream_init_eusart_instances.h"
#include "sl_iostream_stdlib_config.h"
#include "hal.h"
#include "sl_mbedtls.h"
#include "sl_mpu.h"
#include "nvm3_default.h"
#include "sl_simple_led_instances.h"
#include "cpu.h"
#include "cmsis_os2.h"
#include "sl_iostream_init_instances.h"
#include "sl_power_manager.h"

void sl_platform_init(void)
{
  CHIP_Init();
  sl_device_init_nvic();
  sl_device_init_dcdc();
  sl_hfxo_manager_init_hardware();
  sl_device_init_hfxo();
  sl_device_init_clocks();
  sl_device_init_emu();
  halInit();
  nvm3_initDefault();
  CPU_Init();
  osKernelInitialize();
  sl_power_manager_init();
}

void sl_kernel_start(void)
{
  osKernelStart();
}

void sl_driver_init(void)
{
  sl_simple_led_init_instances();
}

void sl_service_init(void)
{
  sl_sleeptimer_init();
  sl_hfxo_manager_init();
  sl_iostream_stdlib_disable_buffering();
  sl_mbedtls_init();
  sl_mpu_disable_execute_from_ram();
  sl_iostream_init_instances();
}

void sl_stack_init(void)
{
  sl_rail_util_pa_init();
  emberAfPluginCmsisRtosIpcInit();
}

void sl_internal_app_init(void)
{
  app_log_init();
}

void sl_iostream_init_instances(void)
{
  sl_iostream_eusart_init_instances();
}

