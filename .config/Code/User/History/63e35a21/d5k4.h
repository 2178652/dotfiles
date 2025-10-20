#ifndef BUZZER_H
#define BUZZER_H

// --- Pin assignment ---
// Change this if you move the buzzer to another GPIO
#define BUZZER_GPIO 15

// To play melodie when winning. Generated with the help of AI
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523


// --- API ---
/**
 * @brief Initialize the buzzer (configure LEDC timer + channel).
 */
void buzzer_init(void);

/**
 * @brief Turn the buzzer on with a gentle duty cycle.
 */
void buzzer_on(void);

/**
 * @brief Turn the buzzer off (silent).
 */
void buzzer_off(void);

/**
 * @brief Allow to play a note
 */
void buzzer_play_tone(int freq, int duration_ms);

/**
 * @brief Play tone to make a victory jingle
 */
void buzzer_play_victory();

#endif // BUZZER_H
