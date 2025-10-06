#include <string.h>
#include <stdbool.h> 
#include "sdkconfig.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "s1.h"

#define ADG819_CTRL_PIN 25   // GPIO connected to ADG819 IN pin
#define Timer1 2000           // in milliseconds
#define Timer2 1000           // in milliseconds


// Function prototype
void ADG819_Set(bool connectToS1);

void app_main(void)
{
    // Initialize GPIO
    gpio_reset_pin(ADG819_CTRL_PIN);
    gpio_set_direction(ADG819_CTRL_PIN, GPIO_MODE_OUTPUT);

    while (1) {
        // Connect COM ↔ S1
        ADG819_Set(true);
        printf("Switching: COM ↔ S1");
        vTaskDelay(pdMS_TO_TICKS(Timer1));  // convert ms to ticks

        // Connect COM ↔ S2
        ADG819_Set(false);
        printf("Switching: COM ↔ S2");
        vTaskDelay(pdMS_TO_TICKS(Timer2));  // convert ms to ticks
    }
}

void ADG819_Set(bool connectToS1)
{
    // ESP-IDF uses 0/1 instead of LOW/HIGH
    gpio_set_level(ADG819_CTRL_PIN, connectToS1 ? 0 : 1);
    if(connectToS1)
      {start_s1();}
    else{
       //S2 : ENERGY HARVESTING
    }

}
