/**
 * @file led.h
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 0.1
 * @date 2022-05-12
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SRC_LED_H_
#define SRC_LED_H_

#include <stdint.h>

#include "msp430.h"

typedef enum{
    LED_ACTIVE,
    LED_ON,
    LED_OFF,
    LED_HOLD,
    LED_IDLE
}Led_state;

typedef struct {
  uint16_t led_timer;
  uint16_t delay_ms;
  uint16_t repeat;
} led_Blink_t;

typedef struct {
  volatile unsigned char *led_port;
  uint8_t led_pin;
  led_Blink_t pattern;
  Led_state state;
} led_t;

/**
 * Constructs a led object with the following attributes
 * @param led
 * @param port
 * @param pin
 */
uint8_t led_init(led_t *led, uint8_t port, uint8_t pin);

/**
 * A simple state machine function for the LED operation
 * @param led
 */
void led_tick(led_t *led);

/**
 * A function that blinks an LED using a predefined pattern.
 * @param led
 */
uint8_t led_blink(led_t *led);

/**
 * LED toggle
 * @param led
 */
void led_toggle(led_t *led);

/**
 * A functions that sets an LED's state(ON-OFF)
 * @param led
 * @param state
 */
uint8_t led_setState(led_t *led, Led_state state);

/**
 *
 * @param led
 * @return
 */
uint8_t led_getState(led_t *led);

#endif /* SRC_LED_H_ */
