#ifndef _VALIDATION_LED_H
#define _VALIDATION_LED_H

#include <stdint.h>

#define VALID_LED CONFIG_VALID_LED_GPIO
#define INVALID_LED CONFIG_INVALID_LED_GPIO

extern void init_validation_leds(void);
// led is actually the led's gpio but this is hidden using the macro
extern void blink_validation_led(uint32_t led);

#endif
