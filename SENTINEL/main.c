#include "sl_component_catalog.h"
#include "sl_system_init.h"
#include "sl_power_manager.h"
#include "app_init.h"
#include "app_process.h"
#include "app_ipc.h"
#include "sl_system_kernel.h"

int main(void)
{
  sl_system_init();
  ipcInitThread();
  sl_system_kernel_start();
}
