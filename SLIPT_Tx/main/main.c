#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
 
#define GPIO_OUTPUT_IO_0 26
#define GPIO_OUTPUT_PIN_SEL (1ULL<<GPIO_OUTPUT_IO_0)
 
void app_main(void)
{
    // Configure pin 26 as output
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;      // no interrupts
    io_conf.mode = GPIO_MODE_OUTPUT;            // set as output mode
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL; // pin mask
    io_conf.pull_down_en = 0;                   // disable pull-down
    io_conf.pull_up_en = 0;                     // disable pull-up
    gpio_config(&io_conf);
 
    while (1) {
        // Turn GPIO26 ON
        gpio_set_level(GPIO_OUTPUT_IO_0, 1);
        vTaskDelay(10000 / portTICK_PERIOD_MS);
 
        // Turn GPIO26 OFF
        gpio_set_level(GPIO_OUTPUT_IO_0, 0);
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}
 
 
 