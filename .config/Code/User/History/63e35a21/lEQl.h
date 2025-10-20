#ifndef BUZZER_H
#define BUZZER_H

// --- Pin assignment ---
// Change this if you move the buzzer to another GPIO
#define BUZZER_GPIO 15

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

#endif // BUZZER_H
