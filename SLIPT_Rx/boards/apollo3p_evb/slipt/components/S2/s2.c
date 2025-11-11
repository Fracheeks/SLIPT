#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"
#include "s2.h"
#include "FreeRTOS.h"
#include "task.h"

#define ADC_SLOT           0     // ADC slot 0
#define ADC_SAMPLE_DELAY   100   // ms between samples

TaskHandle_t ADCTaskHandle = NULL;
am_hal_adc_config_t g_sADCConfig;
void *g_ADCHandle;

static bool g_bADCTaskRunning = false;

void ADCTask(void *pvParameters);
void start_s2(void);
void stop_s2(void); 
void display_ADC_values(uint32_t ui32Sample);;

const am_hal_gpio_pincfg_t g_sPinCfgADC =
{
    .uFuncSel = AM_HAL_PIN_13_ADCD0PSE8, 
    
    .eGPInput = AM_HAL_GPIO_PIN_INPUT_ENABLE,

    .ePullup = AM_HAL_GPIO_PIN_PULLUP_NONE,
};

void ADCTask(void *pvParameters)
{
    // 1. GPIO CONFIGURATION: Configure GPIO 13 (the physical pin) as the ADC input (Channel SE8).

    am_hal_gpio_pinconfig(13, g_sPinCfgADC);

    // Initialize and power up the ADC module
    am_hal_adc_initialize(0, &g_ADCHandle);
    am_hal_adc_power_control(g_ADCHandle, AM_HAL_SYSCTRL_WAKE, false);

    // ADC configuration (Global settings)
    g_sADCConfig.eTrigger = AM_HAL_ADC_TRIGSEL_SOFTWARE;
    g_sADCConfig.eReference = AM_HAL_ADC_REFSEL_INT_2P0; 
    g_sADCConfig.eClockMode = AM_HAL_ADC_CLKMODE_LOW_POWER;
    g_sADCConfig.eRepeat = AM_HAL_ADC_SINGLE_SCAN;
    g_sADCConfig.ePowerMode = AM_HAL_ADC_LPMODE1;
    am_hal_adc_configure(g_ADCHandle, &g_sADCConfig);

    // 2. SLOT CONFIGURATION: Select Channel 8 (SE8), which is mapped to GPIO 13.
    am_hal_adc_slot_config_t sSlotCfg = {
        .bEnabled = true,
        .eMeasToAvg = AM_HAL_ADC_SLOT_AVG_1, // No averaging
        .eChannel = AM_HAL_ADC_SLOT_CHSEL_SE8, 
        .ePrecisionMode = AM_HAL_ADC_SLOT_12BIT,
        .bWindowCompare = false,
    };
    am_hal_adc_configure_slot(g_ADCHandle, ADC_SLOT, &sSlotCfg);

    am_hal_adc_enable(g_ADCHandle);

    g_bADCTaskRunning = true;

    while (g_bADCTaskRunning)
    {
        // Software trigger for the conversion
        am_hal_adc_sw_trigger(g_ADCHandle);

        am_hal_adc_sample_t Sample;
        uint32_t ui32NumSamples = 1;
        
        // Read the sample, waiting for the conversion to finish
        am_hal_adc_samples_read(g_ADCHandle, true, NULL, &ui32NumSamples, &Sample);

        // Display the raw and calculated voltage values
        display_ADC_values(Sample.ui32Sample);

        // Wait for the defined delay before the next sample
        vTaskDelay(pdMS_TO_TICKS(ADC_SAMPLE_DELAY));
    }

        // Cleanup code 
        am_hal_adc_disable(g_ADCHandle);
        am_hal_adc_deinitialize(g_ADCHandle);
        am_hal_gpio_pinconfig(13, g_AM_HAL_GPIO_DISABLE); // Disable GPIO to save power

        ADCTaskHandle = NULL; // Clear the handle
        vTaskDelete(NULL);    // Self-delete the task
    }

void display_ADC_values(uint32_t ui32Sample){
        
        // Print the raw 12-bit ADC value
        am_util_stdio_printf("ADC raw: %d\n", ui32Sample);
        
        // Calculate and print the voltage in millivolts (mV).
        // Scaling formula: (Raw Value / 4095.0) * Reference Voltage (2000 mV)
        am_util_stdio_printf("ADC voltage: %.2f mV\n\n", (ui32Sample / 4095.0f) * 2000.0f); 

}


void start_s2(void){
    
    if (ADCTaskHandle == NULL) { // Only create the task if it's not already running
        xTaskCreate(ADCTask, "ADC Task", 1024, NULL, 2, &ADCTaskHandle); 
    };
}

void stop_s2(void){

    // Stop the task safely by setting the flag to false
    if (ADCTaskHandle != NULL) {
        g_bADCTaskRunning = false;
    }
}