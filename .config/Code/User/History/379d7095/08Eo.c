#include "driver/ledc.h"
#include "buzzer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void buzzer_init(void) {
    ledc_timer_config_t timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_10_BIT,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = 2000,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t channel = {
        .gpio_num   = BUZZER_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = LEDC_CHANNEL_0,
        .intr_type  = LEDC_INTR_DISABLE,
        .timer_sel  = LEDC_TIMER_0,
        .duty       = 0,
        .hpoint     = 0
    };
    ledc_channel_config(&channel);
}

void buzzer_on(void) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 16);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void buzzer_off(void) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}

void buzzer_play_tone(int freq, int duration_ms) {
    ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, freq);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 512);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    vTaskDelay(pdMS_TO_TICKS(duration_ms));

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    vTaskDelay(pdMS_TO_TICKS(50));
}

static void buzzer_play_victory() {
    // Step 1: Boot sequence – slow low tones
    buzzer_play_tone(NOTE_C3, 300);
    buzzer_play_tone(NOTE_D3, 300);
    buzzer_play_tone(NOTE_E3, 300);

    // Step 2: Power rising – ascending tones
    buzzer_play_tone(NOTE_G3, 200);
    buzzer_play_tone(NOTE_A3, 200);
    buzzer_play_tone(NOTE_B3, 200);
    buzzer_play_tone(NOTE_C4, 300);

    // Step 3: Subsystems initializing – arpeggios
    for (int i = 0; i < 3; i++) {
        buzzer_play_tone(NOTE_C4, 150);
        buzzer_play_tone(NOTE_E4, 150);
        buzzer_play_tone(NOTE_G4, 150);
    }

    // Step 4: Core systems sync – rhythmic tones
    buzzer_play_tone(NOTE_D4, 200);
    buzzer_play_tone(NOTE_F4, 200);
    buzzer_play_tone(NOTE_A4, 300);
    buzzer_play_tone(NOTE_G4, 300);

    // Step 5: Life support restored – victory chords
    for (int i = 0; i < 2; i++) {
        buzzer_play_tone(NOTE_C4, 150);
        buzzer_play_tone(NOTE_E4, 150);
        buzzer_play_tone(NOTE_G4, 150);
        buzzer_play_tone(NOTE_C5, 300);
    }

    // Step 6: Final confirmation tone
    buzzer_play_tone(NOTE_E5, 200);
    buzzer_play_tone(NOTE_G5, 400);

    vTaskDelete(NULL);  // End the task
}

void buzzer_start_victory(void)
{
    xTaskCreate( buzzer_play_victory, "buzzer_victory", 2048, NULL, 5, NULL );
}
