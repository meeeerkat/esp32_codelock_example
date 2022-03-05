#ifndef _VALIDATION_LED_H_
#define _VALIDATION_LED_H_

#include <stdint.h>

#define SUCCESS_LED CONFIG_SUCCESS_LED_GPIO
#define FAILURE_LED CONFIG_FAILURE_LED_GPIO

extern void init_validation_leds(void);
// led is actually the led's gpio but this is hidden using the macro
extern void blink_validation_led(uint32_t led);

#endif
