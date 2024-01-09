/**
 * main.c
 * Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * 
 * @version 1.0
 * @date 2022-06-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <msp430.h>
#include <stdint.h>

#include "adc.h"
#include "battery.h"
#include "config.h"
#include "encoder.h"
#include "gpio.h"
#include "gpio_peripherals.h"
#include "helpers.h"
#include "led.h"
#include "max5418.h"
#include "message_handler.h"
#include "test_board_leds.h"
#include "timer.h"
#include "uart.h"

message_t CmdMessage;

extern volatile uint8_t transEnd_flag;  // UART transmission end flag.
led_t status_led;
static void init(void);

void main(void) {
  init();  // Initialize peripherals
  while (1) {
#ifndef _DEBUG_
    watchdog_kick();
#endif
    if (transEnd_flag) {  // If a specific message-end char has been received..
      uart_read_RB(CmdMessage.m_buffer, FTDI);  // Reads the message..
      if (parse_message(&CmdMessage)) {  // Parses the message and if the layout
                                         // is correct..
        process_message(&CmdMessage);  // Process the message to respond to the
                                       // requested action..
        uart_write_RB(CmdMessage.m_buffer,
                      FTDI);  // Return the result of the action..
      }
      clean_message_struct(&CmdMessage);  // And finally clean the structure
                                          // that holds the message.
    }
    led_tick(&status_led);  // Ticks the device's status LED.
  }
}

static void init(void) {
  //##### Configure the clock module - MCLK = 1MHz ######
  watchdog_stop();
  UCSCTL3 |= SELREF_2;      // Set DCO FLL reference = REFO
  UCSCTL4 |= SELA_0;        // Set ACLK = XT1CLK
  __bis_SR_register(SCG0);  // Disable the FLL control loop
  UCSCTL0 = 0x0000;         // Set lowest possible DCOx, MODx
  UCSCTL1 = DCORSEL_0;       // Select DCO range 1MHz operation
  UCSCTL2 = FLLD_0 + 30;    // (N + 1) * FLLRef = Fdco
                            // (30 + 1) * 32768 = 1MHz
  __bic_SR_register(SCG0);
  _enable_interrupt();
  //################## INIT PERIPHERALS ##################
  adc_init();
  i2cInit_Master();
  i2cInit_Slave();
  uart_init();
  interUart_init();  // Inits the internal UART comm between the bnvd and the
                     // TB.
  timer_init();    // Inits the timer module
  battery_init();  // Inits the battery module with default polarity 1 and
                   // Voltage 1.5V
  enc_init();                   // Inits the encoder module.
  gpio_periph_init();           // Sets the HLSD pin low and Jtag_VDD pin High
  TB_led_init();                // Inits the TestBoard's led module
  m_handler_init(&CmdMessage);  // Inits the message handler
  //######################################################
  // Initialize the status LED with a default pattern
  status_led.state = LED_ACTIVE;
  status_led.pattern.delay_ms = STATUS_IDLE;
  led_init(&status_led, 7, BIT5);
}
