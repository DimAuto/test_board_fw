/**
 * @file timer.h
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 1.0
 * @date 2022-5-18
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include <adc.h>
#include <gpio.h>
#include <led.h>
#include <msp430.h>
#include <stdint.h>
#include <uart.h>

/**
 *Initializes the timer module.
 */
void timer_init(void);

/**
 * Blocking delay function.
 * @param msecs
 */
void delayMS(uint16_t msecs);

/**
 * Returns the current milli counter.
 */
uint16_t millis(void);

/**
 * Returns the ellapsed time in milliseconds
 * @return
 */
int ellapsed_millis(uint16_t st_timer);


void watchdog_kick(void);

void watchdog_stop(void);


#endif /* SRC_TIMER_H_ */
