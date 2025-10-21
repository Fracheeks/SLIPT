#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define LASER_GPIO      26
#define LEDC_CHANNEL    LEDC_CHANNEL_0
#define LEDC_TIMER      LEDC_TIMER_0
#define LEDC_MODE       LEDC_LOW_SPEED_MODE
#define LEDC_FREQUENCY  5000      // 5 kHz PWM carrier frequency
#define LEDC_RESOLUTION LEDC_TIMER_8_BIT

#define SYMBOL_PERIOD_MS 600  // Duration of each 2-bit symbol in milliseconds

// Duty cycles for the 4 intensity levels (0%, 33%, 66%, 100%)
static const uint8_t intensity_levels[4] = {0, 84, 168, 255};

// Function to transmit a single 2-bit symbol
void transmit_symbol(uint8_t symbol) {
    uint8_t duty = intensity_levels[symbol & 0x03];  // mask to 2 bits (0x00000011)
    printf("Symbol: %d%d (duty=%d)\n", (symbol >> 1) & 1, symbol & 1, duty);
 
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    vTaskDelay(SYMBOL_PERIOD_MS / portTICK_PERIOD_MS);  // hold symbol for its duration
}

// Function to transmit a byte (8 bits) as 4 consecutive 2-bit symbols
void transmit_byte(uint8_t byte) {
    // Extract 2 bits at a time, starting from the most significant bits
    transmit_symbol((byte >> 6) & 0x03);
    transmit_symbol((byte >> 4) & 0x03);
    transmit_symbol((byte >> 2) & 0x03);
    transmit_symbol(byte & 0x03);
}

// Function to transmit a string by sending each byte as 4 symbols
void transmit_string(const char *str) {
    for (size_t i = 0; i < strlen(str); i++) {
    printf("%c\n", str[i]);
        transmit_byte((uint8_t)str[i]);
    }
}

void app_main(void) {
    // Configure PWM timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_RESOLUTION,
        .freq_hz          = LEDC_FREQUENCY,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // Configure PWM channel
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LASER_GPIO,
        .duty           = 0,
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    // Message to transmit
    const char *message = "CIAO";

    while (1) {
        printf("Transmitting: %s\n", message);
        transmit_string(message);

        // Turn off the laser for 2 seconds before next transmission
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

/*
Example: Transmitting "CIAO"

Each character is converted to its 8-bit ASCII value, then split into 4 symbols of 2 bits each.
Each 2-bit symbol sets the laser intensity (PWM duty cycle):

Symbol → Duty cycle mapping:
  00 → 0%   (OFF)
  01 → 33%  (LOW)
  10 → 66%  (MEDIUM)
  11 → 100% (HIGH)

Character breakdown:

| Char | 2-bit symbols | PWM Levels (%)        |
|------|----------------|-----------------------|
|  C   | 01 00 00 11    | 33%, 0%, 0%, 100%     |
|  I   | 01 00 10 01    | 33%, 0%, 66%, 33%     |
|  A   | 01 00 00 01    | 33%, 0%, 0%, 33%      |
|  O   | 01 00 11 11    | 33%, 0%, 100%, 100%   |

Each symbol lasts SYMBOL_PERIOD_MS (e.g. 200 ms).
Thus, one character (4 symbols) = 4 × 200 ms = 800 ms total.
After the full message, the laser turns off for 2 seconds.
*/
