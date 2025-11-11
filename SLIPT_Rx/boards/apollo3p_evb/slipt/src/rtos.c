#include "am_mcu_apollo.h"
#include "FreeRTOS.h"
#include "task.h"

// Sleep called from FreeRTOS idle
uint32_t am_freertos_sleep(uint32_t idleTime)
{
    am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
    return 0;
}

// Wakeup hook
void am_freertos_wakeup(uint32_t idleTime)
{
    return;
}

// FreeRTOS error hooks
void vApplicationMallocFailedHook(void)
{
    while (1);
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) pxTask;
    while (1) { __asm("BKPT #0\n"); }
}
