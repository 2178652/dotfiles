#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c_lcd.h"
#include "buzzer.h"
#include "driver/adc.h"

// game.c
static int dark_level   = 2400;
static int bright_level = 4000;
static int sweet_min    = 3400;
static int sweet_max    = 3500;


static void game_task(void *pvParameters) {
    while (1) {
        int raw = adc1_get_raw(ADC_CHANNEL);

        if (raw < sweet_min) {
            lcd_put_cursor(0,0);
            lcd_send_string("Sunlight too low ");
            buzzer_off();
        } else if (raw > sweet_max) {
            lcd_put_cursor(0,0);
            lcd_send_string("!! OVERHEAT !!  ");
            buzzer_on();
        } else {
            lcd_put_cursor(0,0);
            lcd_send_string("Optimal sunlight!");
            buzzer_off();
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void game_start(void) {
    xTaskCreate(game_task, "game_task", 4096, NULL, 5, NULL);
}
