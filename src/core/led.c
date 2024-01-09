/**
 * @file led.c
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 0.1
 * @date 2022-05-12
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "led.h"
#include "gpio.h"
#include "timer.h"


uint8_t led_init(led_t *led, uint8_t port, uint8_t pin) {
  led->led_pin = pin;

  switch (port) {
    case 1:
      P1DIR |= led->led_pin;   // SET PORT BIT AS OUTPUT
      P1OUT &= ~led->led_pin;  // RESET PIN
      led->led_port = &P1OUT;  // ASSIGN PORT PIN TO OBJECT
      break;

    case 2:
      P2DIR |= led->led_pin;
      P2OUT &= ~led->led_pin;
      led->led_port = &P2OUT;
      break;

    case 3:
      P3DIR |= led->led_pin;
      P3OUT &= ~led->led_pin;
      led->led_port = &P3OUT;
      break;

    case 4:
      P4DIR |= led->led_pin;
      P4OUT &= ~led->led_pin;
      led->led_port = &P4OUT;
      break;

    case 5:
      P5DIR |= led->led_pin;
      P5OUT &= ~led->led_pin;
      led->led_port = &P5OUT;
      break;

    case 6:
      P6DIR |= led->led_pin;
      P6OUT &= ~led->led_pin;
      led->led_port = &P6OUT;
      break;

    case 7:
      P7DIR |= led->led_pin;
      P7OUT &= ~led->led_pin;
      led->led_port = &P7OUT;
      break;

    case 8:
      P8DIR |= led->led_pin;
      P8OUT &= ~led->led_pin;
      led->led_port = &P8OUT;
      break;

    case 9:
      P9DIR |= led->led_pin;
      P9OUT &= ~led->led_pin;
      led->led_port = &P9OUT;
      break;
  }
  return 0;
}

void led_tick(led_t *led){
    switch (led->state){
    case LED_IDLE:
        return;
    case LED_ACTIVE:
        led->pattern.led_timer = millis();
        led_toggle(led);
        led->state = LED_HOLD;
        break;
    case LED_HOLD:
        if (ellapsed_millis(led->pattern.led_timer) > led->pattern.delay_ms){
            led->state = LED_ACTIVE;
        }
        break;
    default:
        break;
    }
}

uint8_t led_setState(led_t *led, Led_state state) {
  led->state = state;
  if (led->state == LED_OFF) {
    *(led->led_port) &= ~led->led_pin;
  } else if (led->state == LED_ON) {
    *(led->led_port) |= led->led_pin;
  }
  return 0;
}

void led_toggle(led_t *led) { *(led->led_port) ^= led->led_pin; }

uint8_t led_getState(led_t *led) { return led->state; }

uint8_t led_blink(led_t *led) {
  uint8_t i = led->pattern.repeat * 2;
  for (i; i > 1; i--) {
    led_toggle(led);
    delayMS(led->pattern.delay_ms);
  }
  return 0;
}
