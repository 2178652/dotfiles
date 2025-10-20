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
    lcd_put_cursor(0, 0);
    lcd_send_string("Light Gauge:");

    // Init ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11);

    while (1) {
        int raw = adc1_get_raw(ADC_CHANNEL);

        // Normalize to 0–16 blocks (for a 16‑char LCD row)
        int level = (raw * 16) / 4095;

        // Build gauge string
        char gauge[17];
        for (int i = 0; i < 16; i++) {
            gauge[i] = (i < level) ? '#' : '-';
        }
        gauge[16] = '\0';

        // Display on second row
        lcd_put_cursor(1, 0);
        lcd_send_string(gauge);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
