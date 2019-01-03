/** @file badge_disobey_samd.h */
#ifndef BADGE_DISOBEY_SAMD_H
#define BADGE_DISOBEY_SAMD_H

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>

#define DISOBEY_SAMD_BUTTON_LEFT   0
#define DISOBEY_SAMD_BUTTON_UP     1
#define DISOBEY_SAMD_BUTTON_BACK   2
#define DISOBEY_SAMD_BUTTON_OK     3
#define DISOBEY_SAMD_BUTTON_DOWN   4
#define DISOBEY_SAMD_BUTTON_RIGHT  5

__BEGIN_DECLS

/** interrupt handler type */
typedef void (*badge_disobey_samd_intr_t)(void*, bool);

/** touch info */
struct badge_disobey_samd_touch_info {
	/** bitmapped touch state */
	uint32_t touch_state;
};

/**
 * Initialize internal structures and interrupt-handling for the disobey_samd.
 * @note This should be called before using any other methods.
 *   The only exception is badge_disobey_samd_set_interrupt_handler().
 * @return ESP_OK on success; any other value indicates an error
 */
extern esp_err_t badge_disobey_samd_init(void);

/**
 * Configure interrupt handler for a specific pin.
 * @param pin the pin-number on the disobey_samd chip.
 * @param handler the handler to be called on an interrupt.
 * @param arg the argument passed on to the handler.
 * @note It is safe to set the interrupt handler before a call to badge_disobey_samd_init().
 */
extern void badge_disobey_samd_set_interrupt_handler(uint8_t pin, badge_disobey_samd_intr_t handler, void *arg);

/**
 * Retrieve the disobey_samd status.
 * @return the status registers; or -1 on error
 */
extern int badge_disobey_samd_get_interrupt_status(void);

/**
 * Retrieve disobey_samd touch info
 * @param info touch info will be written to this structure.
 * @return ESP_OK on success; any other value indicates an error
 */
extern esp_err_t badge_disobey_samd_get_touch_info(struct badge_disobey_samd_touch_info *info);

/**
 * Read raw touch, usb and battery statusses
 * @param None
 * @return ...
 */

extern int badge_disobey_samd_read_state(void);

/**
 * Read touch button status
 * @param None
 * @return one bit per button
 */

extern int badge_disobey_samd_read_touch(void);

/**
 * Read USB connection status
 * @param None
 * @return 1 if connected, 0 if disconnected
 */

extern int badge_disobey_samd_read_usb(void);

/**
 * Read battery level
 * @param None
 * @return ...
 */

extern int badge_disobey_samd_read_battery(void);

/**
 * Set backlight PWM value
 * @param PWM value (0-255)
 * @return ESP_OK on success; any other value indicates an error
 */

extern esp_err_t badge_disobey_samd_write_backlight(uint8_t value);

/**
 * Set led color
 * @param led (0-5), red (0-255), green (0-255) and blue (0-255)
 * @return ESP_OK on success; any other value indicates an error
 */

extern esp_err_t badge_disobey_samd_write_led(uint8_t led, uint8_t r, uint8_t g, uint8_t b);

/**
 * Set buzzer frequency and duration
 * @param frequency, duration (0 is forever)
 * @return ESP_OK on success; any other value indicates an error
 */

extern esp_err_t badge_disobey_samd_write_buzzer(uint16_t freqency, uint16_t duration);

/**
 * Turn off LEDs, buzzer and backlight
 * @param None
 * @return ESP_OK on success; any other value indicates an error
 */

extern esp_err_t badge_disobey_samd_write_off(void);

__END_DECLS

#endif // BADGE_disobey_samd_H
