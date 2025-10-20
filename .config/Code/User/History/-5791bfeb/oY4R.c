#include <stdio.h>
#include <string.h>
#include "driver/adc.h"
#include "i2c_lcd.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "buzzer.h"
#include "game.h"

#define ADC_CHANNEL ADC1_CHANNEL_6

typedef enum {
    STATE_NORMAL,
    STATE_OVERHEAT_PENDING,
    STATE_COOLDOWN,
    STATE_VICTORY
} game_state_t;

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

    game_state_t state = STATE_NORMAL;
    int overheat_ticks = 0;
    int perfect_ticks = 0;

    while (1) {
        int raw = adc1_get_raw(ADC_CHANNEL);

        switch (state) {
        case STATE_NORMAL:
            if (raw > sweet_max) {
                overheat_ticks++;
                if (overheat_ticks >= 4) {
                    state = STATE_COOLDOWN;
                    overheat_ticks = 0;
                    perfect_ticks = 0;
                }
            } else {
                overheat_ticks = 0;
            }

            if (raw >= sweet_min && raw <= sweet_max) {
                perfect_ticks++;
                if (perfect_ticks >= 10) {
                    state = STATE_VICTORY;
                    buzzer_off();
                    lcd_clear();
                }
            } else {
                perfect_ticks = 0;
            }

            if (raw < dark_level) raw = dark_level;
            if (raw > bright_level) raw = bright_level;
            int level = ((raw - dark_level) * 16) / (bright_level - dark_level);

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

            char gauge[17];
            for (int i = 0; i < 16; i++) {
                gauge[i] = (i < level) ? '#' : '-';
            }
            gauge[16] = '\0';
            lcd_put_cursor(1, 0);
            lcd_send_string(gauge);

            break;

        case STATE_COOLDOWN:
            for (int i = 4; i > 0; i--) {
                lcd_put_cursor(0, 0);
                char msg[17];
                snprintf(msg, sizeof(msg), "Cooling... %d ", i);
                lcd_send_string(msg);

                lcd_put_cursor(1, 0);
                lcd_send_string("----------------");

                buzzer_off();
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
            state = STATE_NORMAL;
            break;

        case STATE_VICTORY: {
            const char *banner = "Solar panel online, electric cooling system restored. Well done captain";

            lcd_scroll_banner(banner, 0, 200);

            lcd_put_cursor(1, 0);
            lcd_send_string("OXYGEN RESTORED ");

            while (1) {
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
            break;
        }


        default:
            state = STATE_NORMAL;
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void game_start(void)
{
    xTaskCreate(game_task, "game_task", 4096, NULL, 5, NULL);
}
