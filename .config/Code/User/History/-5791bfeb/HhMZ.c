#include <stdio.h>
#include "driver/adc.h"
#include "i2c_lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "buzzer.h"
#include "game.h"

#define ADC_CHANNEL ADC1_CHANNEL_6 // GPIO34

// Thresholds (can later be made configurable)
static int dark_level   = 2400;
static int bright_level = 4000;
static int sweet_min    = 3400;
static int sweet_max    = 3500;

static void game_task(void *pvParameters)
{
    lcd_init();
    lcd_clear();

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11);

    buzzer_init();

    while (1) {
        int raw = adc1_get_raw(ADC_CHANNEL);

        // Clamp
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
            buzzer_on();
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

void game_start(void)
{
    xTaskCreate(game_task, "game_task", 4096, NULL, 5, NULL);
}
