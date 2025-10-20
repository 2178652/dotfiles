typedef enum {
    STATE_NORMAL,
    STATE_OVERHEAT_PENDING,
    STATE_COOLDOWN,
    STATE_VICTORY
} game_state_t;

static game_state_t state = STATE_NORMAL;
static int overheat_ticks = 0;
static int perfect_ticks = 0;

while (1) {
    int raw = adc1_get_raw(ADC_CHANNEL);

    switch (state) {
    case STATE_NORMAL:
        // --- Overheat detection ---
        if (raw > perfect_max) {
            overheat_ticks++;
            if (overheat_ticks >= 4) { // 2s overheated
                state = STATE_COOLDOWN;
                overheat_ticks = 0;
                perfect_ticks = 0;
            }
        } else {
            overheat_ticks = 0;
        }

        // --- Perfect zone detection ---
        if (raw >= perfect_min && raw <= perfect_max) {
            perfect_ticks++;
            if (perfect_ticks >= 10) { // 5s perfect
                state = STATE_VICTORY;
                buzzer_off();
                lcd_clear();
            }
        } else {
            perfect_ticks = 0;
        }

        // Clamp and map
        if (raw < dark_level) raw = dark_level;
        if (raw > bright_level) raw = bright_level;
        int level = ((raw - dark_level) * 16) / (bright_level - dark_level);

        // Display
        lcd_put_cursor(0, 0);
        if (raw < perfect_min) {
            lcd_send_string("Sunlight too low ");
            buzzer_off();
        } else if (raw > perfect_max) {
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
        const char *banner = "Solar panel online, electric cooling system restored! Great job captain";
        int len = strlen(banner);

        for (int offset = 0; offset < len - 15; offset++) {
            lcd_put_cursor(0, 0);
            char window[17];
            strncpy(window, banner + offset, 16);
            window[16] = '\0';
            lcd_send_string(window);

            lcd_put_cursor(1, 0);
            lcd_send_string("OXYGEN RESTORED ");

            vTaskDelay(pdMS_TO_TICKS(300));
        }

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
