/**
 * @file uart.c
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 0.1
 * @date 2022-06-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "ring_buffer.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "uart.h"
#include "timer.h"

RB_t RB_uart_rx;
RB_t RB_uart_tx;

volatile uint8_t transEnd_flag = 0;
volatile uint8_t intTransEnd_flag = 0;
uint8_t *transEnd_flag_ptr;
// uint8_t uart_select = FTDI;  // Selection flag for the desirable uart.
volatile unsigned char *UART_IE_VEC;
//static volatile uint8_t tmp;
uint8_t start_char = UART_START_CHAR;
uint8_t stop_char = UART_STOP_CHAR;
uint8_t buffer_size = UART_RB_SIZE;
uint8_t pass_through_flag = 0;
static volatile uint16_t ota_timeout = 0;
uint8_t ota_complete = 0;
// #################### UART FUNCTIONS ####################
void uart_init(void) {
  RB_init(&RB_uart_rx, UART_RB_SIZE);  // Init RingBuffer object
  RB_init(&RB_uart_tx, UART_RB_SIZE);

  UART_FTDI_SEL |= (UART_FTDI_RX_PIN | UART_FTDI_TX_PIN);
  UART_FTDI_DIR |= (UART_FTDI_RX_PIN | UART_FTDI_TX_PIN);

//  uint16_t div_factor =
//      (((float)clock_freq_mhz) / ((float)baud_rate)) * 1000000;
//  uint8_t ucbrsx = ((((float)clock_freq_mhz) / ((float)baud_rate)) * 1000000 -
//                    (float)div_factor) *
//                       8 +
//                   0.5;

  UCA1CTL1 |= UCSWRST;
  UCA1CTL1 |= UCSSEL_2;  // USCI clock source set to SMCLK

  UCA1BR0 = 0x68;
  UCA1BR1 = 0;
  UCA1MCTL = 0x02;;
  // UCA1MCTL = UCBRS_1;

  UCA1CTL1 &= ~UCSWRST;  // Clear SW reset (USCI reset released for operation).

  UCA1IE |= UCAx_RX_EN;  // Enable  USCI_A0 RX and TX interrupts
}


void interUart_init(void) {
  PMAPPWD = 0x02D52;
  // I2C Pins
  P2SEL |= (BIT4 | BIT5);
  P2DIR |= (BIT4 | BIT5);
  P2MAP4 = PM_UCA0TXD;
  P2MAP5 = PM_UCA0RXD;

  PMAPPWD = 0;

  UCA0CTL1 |= UCSWRST;
  UCA0CTL1 |= UCSSEL_2;  // USCI clock source set to SMCLK
  UCA0MCTL = UCBRS_1;
  UCA0BR0 = 0x68;
  UCA0BR1 = 0;
  UCA0MCTL = 0x02;
  UCA0CTL1 &= ~UCSWRST;  // Clear SW reset (USCI reset released for operation).

  UCA0IE |= UCAx_RX_EN;  // Enable  USCI_A0 RX and TX interrupts
}

void uart_select(uint8_t uart) {
  if (uart) {
    UCA1IE = 0;
    UCA0IE |= UCAx_RX_EN;  // Enable  USCI_A0 RX and TX interrupts
    UART_IE_VEC = &UCA0IE;
    transEnd_flag_ptr = &intTransEnd_flag;
    start_char = INT_UART_START_CHAR;

  } else {
    UCA0IE = 0;
    UCA1IE |= UCAx_RX_EN;  // Enable  USCI_A0 RX and TX interrupts
    UART_IE_VEC = &UCA1IE;
    transEnd_flag_ptr = &transEnd_flag;
    start_char = UART_START_CHAR;
  }
}

static inline uint8_t uart_RXdata_ready(void) { return (UCA1IFG&UCRXIFG); }

static inline uint8_t uart_TXdata_ready(void) { return (UCA1IFG&UCTXIFG); }

static inline  uint8_t interUart_RXdata_ready(void) {
  return (UCA0IFG & UCRXIFG);
}

static inline uint8_t interUart_TXdata_ready(void) {
  return (UCA0IFG & UCTXIFG);
}

uint8_t uart_RB_isFull(void) { return RB_isFull(&RB_uart_rx); }

uint8_t uart_RB_isEmpty(void) { return RB_isEmpty(&RB_uart_rx); }

void uart_clear_RB(void) {
  RB_clear(&RB_uart_rx);
  RB_clear(&RB_uart_tx);
}

uint8_t uart_sizeofRB(void) { return RB_size(&RB_uart_rx); }

uint8_t uart_read_RB(char *buffer, uint8_t uart_sel) {

  uart_select(uart_sel);
  memset(buffer, 0, buffer_size);
  if (RB_pop(&RB_uart_rx) !=
      start_char) {         // Checks whether the received message starts with
                            // the proper char.
    RB_clear(&RB_uart_rx);  // If no, the message gets rejected.
    *transEnd_flag_ptr = 0;
    *UART_IE_VEC |= UCAx_RX_EN;
    return 1;
  }
  while (!(RB_isEmpty(&RB_uart_rx))) {  // Loads the message to the buffer.

    *buffer = RB_pop(&RB_uart_rx);
    buffer++;
  }
  RB_clear(&RB_uart_rx);
  *transEnd_flag_ptr = 0;
  *UART_IE_VEC |= UCAx_RX_EN;  // Re-enable RX interrupt when rb is empty.
  return 0;
}

uint8_t uart_write_RB(char *buffer, uint8_t uart_sel) {
  uart_select(uart_sel);
  while (*buffer !=
         '\0') {  // Loads the message to the transmitters ring buffer.
    RB_push(&RB_uart_tx, *buffer);
    buffer++;
  }
  *transEnd_flag_ptr = 0;
  *UART_IE_VEC |= UCAx_TX_EN;
  return 0;
}

uint8_t uart_pop_RX(void) { return RB_pop(&RB_uart_rx); }

uint8_t uart_popBack_RX(void) { return RB_popBack(&RB_uart_rx); }

void uart_push_TX(uint8_t byte) { return RB_push(&RB_uart_tx, byte); }

void uart_pushFront_TX(uint8_t byte) { return RB_pushFront(&RB_uart_tx, byte); }

uint8_t uart_pass_through(void){
    led_t ota_led;
    ota_led.state = LED_ACTIVE;
    ota_led.pattern.delay_ms = 100;
    led_init(&ota_led, 6, BIT7);
    ota_timeout = millis();
    pass_through_flag = 1;
    UCA0CTL0 |= (UCPEN | UCPAR);
    UCA1CTL0 |= (UCPEN | UCPAR);
    UCA1IE |= UCRXIE;
    UCA0IE |= UCRXIE;
    while (1){
        #ifndef _DEBUG_
            watchdog_kick();
        #endif
        led_tick(&ota_led);
        if (ellapsed_millis(ota_timeout) > 5000) {
            UCA0CTL0 &= ~(UCPEN | UCPAR);
            UCA1CTL0 &= ~(UCPEN | UCPAR);
            pass_through_flag = 0;
            ota_complete = 1;
            led_setState(&ota_led, LED_OFF);
            return 0;
        }
    }
}

void jump_BSL(void){
    P8SEL = 0x00;
    P8DIR = 0x00;
    delayMS(600);
    _disable_interrupt(); // disable interrupts
    ((void ( * )())0x01000)();   // jump to BSL
}


#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void) {
  // uint8_t tmp_uart_buffer;
  uint8_t tmp = 0;
  switch (__even_in_range(UCA1IV, 4)) {
    case 0x00:  // Vector 0: No interrupts
      break;
    case 0x02:
      if (uart_RXdata_ready) {
        if (pass_through_flag == 0) {
            tmp = UCA1RXBUF;
            RB_push(&RB_uart_rx, tmp);
            if (tmp == CR) {  // If the received char is Enter or the msg stop char,
              UCA1IE &= ~UCAx_RX_EN;  // disable the interrupts
              transEnd_flag = 1;      // and set the transmission-end flag.
            }
            if (RB_isFull(
                    &RB_uart_rx)) {  // If the RX ring buffer is full do the same.
              UCA1IE &= ~UCAx_RX_EN;
              transEnd_flag = 1;
            }
        }
        else {
            UCA0TXBUF = UCA1RXBUF;
            ota_timeout = millis();
        }
      }
      break;

    case 0x04:
      if (uart_TXdata_ready) {  /// If uart raised the interrupt
        UCA1TXBUF = RB_pop(&RB_uart_tx);
        if (RB_isEmpty(&RB_uart_tx)) {
          RB_clear(&RB_uart_tx);
          UCA1IE &= ~UCAx_TX_EN;
        }
      }
      break;
  }
}


#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) {
  // uint8_t tmp_uart_buffer;
  uint8_t tmp = 0;
  switch (__even_in_range(UCA0IV, 4)) {
    case 0x00:  // Vector 0: No interrupts
      break;
    case 0x02:
      if (interUart_RXdata_ready) {
        if (pass_through_flag == 0){
            tmp = UCA0RXBUF;
            RB_push(&RB_uart_rx, tmp);
            if (tmp == CR ||
                tmp == INT_UART_STOP_CHAR) {  // If the received char is Enter or
                                              // the msg stop char,
              UCA0IE &= ~UCAx_RX_EN;          // disable the interrupts
              intTransEnd_flag = 1;           // and set the transmission-end flag.
            }
            if (RB_isFull(
                    &RB_uart_rx)) {  // If the RX ring buffer is full do the same.
              UCA0IE &= ~UCAx_RX_EN;
              intTransEnd_flag = 1;
            }
        }
        else{
            UCA1TXBUF = UCA0RXBUF;
            ota_timeout = millis();
        }
      }
      break;

    case 0x04:
      if (interUart_TXdata_ready) {
        UCA0TXBUF = RB_pop(&RB_uart_tx);
        if (RB_isEmpty(&RB_uart_tx)) {
          RB_clear(&RB_uart_tx);
          UCA0IE &= ~UCAx_TX_EN;
          UCA0IE |= UCAx_RX_EN;
        }
      }
      break;
  }
}
