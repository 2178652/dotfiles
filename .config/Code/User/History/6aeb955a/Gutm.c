#include <stdio.h>
#include "driver/adc.h"
#include "i2c_lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

#define ADC_CHANNEL ADC1_CHANNEL_6  // GPIO34 for example
#define BUZZER_GPIO 15              // Buzzer pin

void buzzer_init(void) {
    // Configure PWM timer
    ledc_timer_config_t timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_10_BIT,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = 2000, // 2 kHz tone
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    // Configure PWM channel
    ledc_channel_config_t channel = {
        .gpio_num   = BUZZER_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = LEDC_CHANNEL_0,
        .intr_type  = LEDC_INTR_DISABLE,
        .timer_sel  = LEDC_TIMER_0,
        .duty       = 0, // start silent
        .hpoint     = 0
    };
    ledc_channel_config(&channel);
}

void app_main(void)
{
    // Init LCD
    lcd_init();
    lcd_clear();

    // Init ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11);

    while (1) {
        // Calibrate based on your sensor readings
        const int dark_level   = 2400;   // baseline in room light
        const int bright_level = 4000;   // flashlight on sensor
        const int sweet_min    = 3400;   // lower bound of "optimal sunlight"
        const int sweet_max    = 3500;   // upper bound of "optimal sunlight"

        int raw = adc1_get_raw(ADC_CHANNEL);

        // Clamp to range
        if (raw < dark_level) raw = dark_level;
        if (raw > bright_level) raw = bright_level;

        // Map to 0–16
        int level = ((raw - dark_level) * 16) / (bright_level - dark_level);

        // --- Top row solar-themed message ---
        lcd_put_cursor(0, 0);
        if (raw < sweet_min) {
            lcd_send_string("Sunlight too low ");
        } else if (raw > sweet_max) {
            lcd_send_string("Overexposed!     ");
        } else {
            lcd_send_string("Optimal sunlight!");
        }

        // --- Bottom row gauge ---
        char gauge[17];
        for (int i = 0; i < 16; i++) {
            gauge[i] = (i < level) ? '#' : '-';
        }
        gauge[16] = '\0';

        lcd_put_cursor(1, 0);
        lcd_send_string(gauge);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


void buzzer_on(void) {
    // Gentle volume: ~12% duty
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 128);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void buzzer_off(void) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void app_main(void)
{
    // Init LCD
    lcd_init();
    lcd_clear();

    // Init ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11);

    // Init buzzer
    buzzer_init();

    while (1) {
        // Calibrate based on your sensor readings
        const int dark_level   = 2400;   // baseline in room light
        const int bright_level = 4000;   // flashlight on sensor
        const int sweet_min    = 3400;   // lower bound of "optimal sunlight"
        const int sweet_max    = 3500;   // upper bound of "optimal sunlight"

        int raw = adc1_get_raw(ADC_CHANNEL);

        // Clamp to range
        if (raw < dark_level) raw = dark_level;
        if (raw > bright_level) raw = bright_level;

        // Map to 0–16
        int level = ((raw - dark_level) * 16) / (bright_level - dark_level);

        // --- Top row solar-themed message ---
        lcd_put_cursor(0, 0);
        if (raw < sweet_min) {
            lcd_send_string("Sunlight too low ");
            buzzer_off();
        } else if (raw > sweet_max) {
            lcd_send_string("!! OVERHEAT !!  ");
            buzzer_on();   // activate buzzer alarm
        } else {
            lcd_send_string("Optimal sunlight!");
            buzzer_off();
        }

        // --- Bottom row gauge ---
        char gauge[17];
        for (int i = 0; i < 16; i++) {
            gauge[i] = (i < level) ? '#' : '-';
        }
        gauge[16] = '\0';

        lcd_put_cursor(1, 0);
        lcd_send_string(gauge);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}