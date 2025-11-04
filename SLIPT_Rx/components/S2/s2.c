// #include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"
#include "FreeRTOS.h"
#include "task.h"
#include "s2.h"

#define ADC_PIN            26    // physical pin connected to solar panel
#define ADC_SLOT           0     // ADC slot 0
#define ADC_SAMPLE_DELAY   100   // ms between samples

TaskHandle_t ADCTaskHandle = NULL;
am_hal_adc_config_t g_sADCConfig;
void *g_ADCHandle;

void ADCTask(void *pvParameters);
void start_s2(void);
void display_ADC_values(uint32_t ui32Sample)


void ADCTask(void *pvParameters)
{
    // Enable ADC module
    am_hal_adc_initialize(0, &g_ADCHandle);
    am_hal_adc_power_control(g_ADCHandle, AM_HAL_SYSCTRL_WAKE, false);

    // ADC configuration
    g_sADCConfig.eClock = AM_HAL_ADC_CLKSEL_HFRC_DIV2;
    g_sADCConfig.ePolarity = AM_HAL_ADC_TRIGPOL_RISING;
    g_sADCConfig.eTrigger = AM_HAL_ADC_TRIGSEL_SOFT;
    g_sADCConfig.eReference = AM_HAL_ADC_REFSEL_INT_2P0; // 2.0V internal reference
    g_sADCConfig.eClockMode = AM_HAL_ADC_CLKMODE_LOW_POWER;
    g_sADCConfig.eRepeat = AM_HAL_ADC_REPEAT_SINGLE;
    g_sADCConfig.ePowerMode = AM_HAL_ADC_LPMODE1;
    am_hal_adc_configure(g_ADCHandle, &g_sADCConfig);

    // ADC slot configuration (select channel and averaging)
    am_hal_adc_slot_config_t sSlotCfg = {
        .bEnabled = true,
        .eMeasToAvg = AM_HAL_ADC_SLOT_AVG_1, // AVG_1 = no averaging, one sample (Change to AVG_N to average multiple samples)
        .eChannel = AM_HAL_ADC_SLOT_CHSEL_SE0,  // ADC channel 0
        .ePrecisionMode = AM_HAL_ADC_SLOT_12BIT,
        .bWindowCompare = false,
    };
    am_hal_adc_configure_slot(g_ADCHandle, ADC_SLOT, &sSlotCfg);

    am_hal_adc_enable(g_ADCHandle);

    while (1)
    {
        am_hal_adc_sw_trigger(g_ADCHandle);

        am_hal_adc_sample_t Sample;
        uint32_t ui32NumSamples = 1;
        am_hal_adc_samples_read(g_ADCHandle, true, NULL, &ui32NumSamples, &Sample);

        display_ADC_values(Sample.ui32Sample);

        vTaskDelay(pdMS_TO_TICKS(ADC_SAMPLE_DELAY));
    }

    am_hal_adc_disable(g_ADCHandle);
    am_hal_adc_deinitialize(g_ADCHandle);
    vTaskDelete(NULL);
}

void display_ADC_values(uint32_t ui32Sample){
        
        am_util_stdio_printf("ADC raw: %d\n", ui32Sample);
        am_util_stdio_printf("ADC voltage: %.2f mV\n\n", (ui32Sample / 4095.0f) * 2000.0f); // Scale to 2.0V ref

}s

void start_s2(void)
{
    xTaskCreate(ADCTask, "ADC Task", 1024, NULL, 2, &ADCTaskHandle); //5th paramter is the priority (higher number = higher priority)
}
