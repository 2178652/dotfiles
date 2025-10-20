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

// Generated using ChatGPT to make a victory sound
static void buzzer_play_victory() {
    // Stage 1: Power-up hum – rising tones, spaced out (system waking)
    buzzer_play_tone(NOTE_C3, 200);
    buzzer_play_tone(NOTE_D3, 200);
    buzzer_play_tone(NOTE_E3, 200);
    vTaskDelay(100);
    
    buzzer_play_tone(NOTE_F3, 150);
    buzzer_play_tone(NOTE_G3, 150);
    buzzer_play_tone(NOTE_A3, 200);
    vTaskDelay(100);

    // Stage 2: Initialization pulses (like systems checking)
    for (int i = 0; i < 3; i++) {
        buzzer_play_tone(NOTE_C4, 100);
        vTaskDelay(50);
        buzzer_play_tone(NOTE_E4, 100);
        vTaskDelay(50);
        buzzer_play_tone(NOTE_G4, 100);
        vTaskDelay(100);
    }

    // Stage 3: Power surge – fast rising scale
    buzzer_play_tone(NOTE_C4, 100);
    buzzer_play_tone(NOTE_D4, 100);
    buzzer_play_tone(NOTE_E4, 100);
    buzzer_play_tone(NOTE_F4, 100);
    buzzer_play_tone(NOTE_G4, 100);
    buzzer_play_tone(NOTE_A4, 100);
    buzzer_play_tone(NOTE_B4, 100);
    buzzer_play_tone(NOTE_C5, 400); // Hold on success tone
    vTaskDelay(200);

    // Stage 4: Confirmed – triumphant arpeggio
    buzzer_play_tone(NOTE_C4, 150);
    buzzer_play_tone(NOTE_E4, 150);
    buzzer_play_tone(NOTE_G4, 150);
    buzzer_play_tone(NOTE_C5, 500); // Final "System online" tone

    vTaskDelete(NULL);
}

void buzzer_start_victory(void)
{
    xTaskCreate( buzzer_play_victory, "buzzer_victory", 2048, NULL, 5, NULL );
}
