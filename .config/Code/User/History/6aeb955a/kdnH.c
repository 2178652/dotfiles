#include <stdio.h>
#include "driver/adc.h"
#include "i2c_lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ADC_CHANNEL ADC1_CHANNEL_6  // GPIO34 for example

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
        const int perfect_min  = 3000;   // lower bound of "perfect"
        const int perfect_max  = 3400;   // upper bound of "perfect"

        int raw = adc1_get_raw(ADC_CHANNEL);

        // Clamp to range
        if (raw < dark_level) raw = dark_level;
        if (raw > bright_level) raw = bright_level;

        // Map to 0–16
        int level = ((raw - dark_level) * 16) / (bright_level - dark_level);

        // --- Top row message ---
        lcd_put_cursor(0, 0);
        if (raw < perfect_min) {
            lcd_send_string("Too little light ");
        } else if (raw > perfect_max) {
            lcd_send_string("!! OVERHEAT !! ");
        } else {
            lcd_send_string("Perfect amount! ");
        }

        // --- Bottom row gauge (optional, keep for feedback) ---
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
