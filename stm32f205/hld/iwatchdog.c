#include "stm32f2xx.h"

void iwatchdog_init(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    IWDG_SetReload(4000); // 30s
    IWDG_ReloadCounter();
    IWDG_Enable();
}

void iwatchdog_feed(void)
{
    IWDG_ReloadCounter();
}
