#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"
#include "s2.h"

#define ADG819_CTRL_PIN 13   // Apollo3 GPIO pin number
#define TIMER1_MS 20000 //in ms
#define TIMER2_MS 10000 //in ms

void ADG819_Set(bool connectToS1);

int main(void)
{
    // Initialize the board
    am_hal_mcuctrl_device_t sDevice;
    am_hal_mcuctrl_info_get(AM_HAL_MCUCTRL_INFO_DEVICEID, &sDevice);

    // Enable printing
    am_bsp_itm_printf_enable();
    am_util_stdio_terminal_clear();
    am_util_stdio_printf("Apollo3 ADG819 Control Example\n");

    // Configure GPIO
    am_hal_gpio_pinconfig(ADG819_CTRL_PIN, g_AM_HAL_GPIO_OUTPUT);

    while (1)
    {
        // Connect COM ↔ S1
        ADG819_Set(true);
        am_util_delay_ms(TIMER1_MS);

        // Connect COM ↔ S2
        ADG819_Set(false);
        am_util_delay_ms(TIMER2_MS);
    }
}

void ADG819_Set(bool connectToS1)
{
    am_util_stdio_printf(connectToS1 == true ? "Switching: COM ↔S2\n" : "Switching: COM ↔ S1\n");
    if (connectToS1)
    {
        am_hal_gpio_output_clear(ADG819_CTRL_PIN); // Logic low
        // S1 : ENERGY HARVESTING
    }
    else
    {
        am_hal_gpio_output_set(ADG819_CTRL_PIN); // Logic high
        start_s2(); // S2 : DATA ANALYSIS 
    }
}

