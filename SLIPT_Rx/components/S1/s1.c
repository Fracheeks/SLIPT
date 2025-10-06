#include <string.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_adc/adc_oneshot.h"
#include "hal/adc_types.h"

#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_cali.h"

/*
This code sample prints out the ADC input in the monitor.
*/

// ONE SHOT MODE : ADC conversion only happens when you instruct it.

TaskHandle_t ADCTaskHandle = NULL;

void ADCTask(void *arg){
    int solar_panel_read, solar_panel_output;
    
    /*                        CREATE ADC UNIT                           */

    adc_oneshot_unit_handle_t handle = NULL;

    adc_oneshot_unit_init_cfg_t init_config1 = {
    .unit_id = ADC_UNIT_2, //ADC_UNIT_1 → ADC1 (channels on GPIO32–39 for ESP32)
                           //ADC_UNIT_2 → ADC2 (channels on GPIO0, 2, 4, 12–15, 25–27 for ESP32)

    .ulp_mode = ADC_ULP_MODE_DISABLE,
    };

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &handle));

    adc_oneshot_chan_cfg_t config = {
    .bitwidth = ADC_BITWIDTH_12, // max resolution is 12 bits, which means a voltage level between 0 and 4095 (2^12 -1)
    .atten = ADC_ATTEN_DB_12, //up to 3.9 V for a Vref of 1.1 Volt
    };

    ESP_ERROR_CHECK(adc_oneshot_config_channel(handle, ADC_CHANNEL_9, &config));

    
    /*                          CALIBRATION                             */

    adc_cali_handle_t cali_handle = NULL;

    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_2,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_11,
    };

    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cali_config, &cali_handle));

    /*          READ RAW ADC INPUT AND CONVERT TO MILIVOLTS             */
    while(1)
    {
        ESP_ERROR_CHECK(adc_oneshot_read(handle, ADC_CHANNEL_9, &solar_panel_read)); // ADC2_CHL 9 , GPIO26 , A0
        printf(" ADC input from solar panel:   %d \n", solar_panel_read);
        adc_cali_raw_to_voltage(cali_handle, solar_panel_read, &solar_panel_output);
        printf(" Milivolt output after calibration %d \n", solar_panel_output);
        printf("\n * \n\n");

        vTaskDelay(100);
    }
    adc_oneshot_del_unit(handle);
    adc_cali_delete_scheme_line_fitting(cali_handle);
    vTaskDelete(NULL);
}

void start_s1(void){
    xTaskCreatePinnedToCore(ADCTask, "ADC Task", 4096, NULL, 10, &ADCTaskHandle, 0);
}