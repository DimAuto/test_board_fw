/**
 * @file timer.c
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 1.0
 * @date 2022-05-18
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "timer.h"

#include "led.h"
#include "src/config.h"

static volatile uint16_t counter;
static volatile uint16_t ellapsed_mill = 0;

void timer_init(void) {
  TB0CCTL0 |= CCIE;
  TB0CTL = TBSSEL_2 + ID_0 + MC_1;  // Select SMCLK, SMCLK/1, Up Mode

  TA2CCTL0 = CCIE;
  TA2CTL = TASSEL_2 + ID_0 + MC_1;
  TA2CCR0 = 1000 - 1;
}

void timer_deinit(void) {
  TA2CCR0 = 0;
  TB0CCR0 = 0;
}

void delayMS(uint16_t msecs) {
  counter = 0;  // Reset Over-Flow counter
  TB0CCR0 =
      1000 -
      1;  // Start Timer, Compare value for Up Mode to get 1ms delay per loop

  while (counter <= msecs)
    ;
  TB0CCR0 = 0;  // Stop Timer
}

void watchdog_stop(void){
    WDTCTL = WDTPW | WDTHOLD;
}

void watchdog_kick(void){
    WDTCTL = WDT_ARST_1000;
}

uint16_t millis(void) { return ellapsed_mill; }

int ellapsed_millis(uint16_t st_timer) {
  int tmp = (ellapsed_mill - st_timer);
  if (tmp < 0) {
    tmp = (UINT16_T_MAX_VALUE - tmp);
    return tmp;
  }
  return tmp;
}

#pragma vector = TIMERB0_VECTOR
__interrupt void TIMERB0_ISR(void) { counter++; }

#pragma vector = TIMER2_A0_VECTOR
__interrupt void TIMER2_A0_ISR(void) {
  ellapsed_mill++;
  if (ellapsed_mill > UINT16_T_MAX_VALUE - 1) {
    ellapsed_mill = 0;
  }
}
