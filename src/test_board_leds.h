/**
 * @file test_board.h
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 1.0
 * @date 2022-07-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SRC_TEST_BOARD_LEDS_H_
#define SRC_TEST_BOARD_LEDS_H_

#include <stdint.h>
#include "msp430.h"
#include "led.h"

/**
 * Initialize the led module
 */
void TB_led_init(void);

/**
 * Set a specific led between 1-5
 * @param led_number
 * @return
 */
uint8_t TB_set_led(uint8_t led_number);

/**
 * Toggles a speific led
 * @param led_number
 * @return
 */
uint8_t TB_toggle_led(uint8_t led_number);

/**
 * Reset a specific led between 1-5
 * @param led_number
 * @return
 */
uint8_t TB_reset_led(uint8_t led_number);

/**
 * Set multiple leds using an 8bit array
 * @param led_array
 * @return
 */
uint8_t TB_multiple_led_set(uint8_t led_array);

/**
 * Reset all leds.
 * @return
 */
uint8_t TB_reset_all(void);

#endif /* SRC_TEST_BOARD_LEDS_H_ */
