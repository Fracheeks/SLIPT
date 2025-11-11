#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"
#include "setup/setup.h" 
#include "s2.h"       
#include "FreeRTOS.h" 
#include "task.h"

// --- Configuration Defines ---
#define ADG819_CTRL_PIN 5   // Apollo3 GPIO pin number
#define TIMER1_MS 20000     // 20 seconds for S1 (Energy Harvesting)
#define TIMER2_MS 10000     // 10 seconds for S2 (Data Analysis)

// Function prototype defined in this file
void ADG819_Set(bool connectToS1);
void MainAppTask(void *pvParameters);

int main(void)
{
    // Core Setup
    setup();

    // Initial Output Check
    am_util_stdio_printf("\n*** System Setup Complete. Starting FreeRTOS. ***\n");

    // Configure Control Pin
    am_hal_gpio_pinconfig(ADG819_CTRL_PIN, g_AM_HAL_GPIO_OUTPUT);

    // Create the main application
    xTaskCreate(
        MainAppTask,                 
        "MainApp",                   
        configMINIMAL_STACK_SIZE,    
        NULL,                        
        tskIDLE_PRIORITY + 1,        
        NULL                         
    );

    // Start the scheduler - this function never returns.
    vTaskStartScheduler();

    // Should never reach here
    while(1);
}

void MainAppTask(void *pvParameters)
{

    (void) pvParameters;

    while (1)
    {
        // --- Phase S1: Energy Harvesting ---
        am_util_stdio_printf("Entering S1 (Energy Harvesting) for %d ms...\n", TIMER1_MS);
        ADG819_Set(true);
        vTaskDelay(pdMS_TO_TICKS(TIMER1_MS));

        // --- Phase S2: Data Analysis ---
        am_util_stdio_printf("Entering S2 (Data Analysis) for %d ms...\n", TIMER2_MS);  
        ADG819_Set(false);
        vTaskDelay(pdMS_TO_TICKS(TIMER2_MS));
    }
}


void ADG819_Set(bool connectToS1)
{
    // Print the current switching action
    am_util_stdio_printf(connectToS1 ? "Switching: COM <-> S1\n" : "Switching: COM <-> S2\n");

    if (connectToS1)
    {
        // S1: Energy Harvesting
        // 1. STOP the ADC task before entering the long S1 phase.
        stop_s2(); 

        // 2. Set the control pin to Logic Low
        am_hal_gpio_output_clear(ADG819_CTRL_PIN); 
    }
    else
    {
        // S2: Data Analysis
        // 1. Set the control pin to Logic High
        am_hal_gpio_output_set(ADG819_CTRL_PIN); 

        // 2. Start the S2 analysis function
        start_s2();
    }
}
