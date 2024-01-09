/**
 * @file uart.h
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 0.1
 * @date 2022-06-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef UART_H_
#define UART_H_

#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>

#define UCAx_TX_EN              2
#define UCAx_RX_EN              1

// UART defines
#define CR                      13
#define UART_START_CHAR         35  // #
#define UART_STOP_CHAR          13  // cr
#define INT_UART_START_CHAR     36  // $
#define INT_UART_STOP_CHAR      46  // .

#define FTDI                    0
#define INTERNAL                1

/**
 * Initialize the usci_a1_uart
 * @param uart_port_sel
 * @param uart_port_dir
 * @param uart_port_out
 * @param rx_pin
 * @param tx_pin
 * @param baud_rate
 * @param clock_freq_mhz
 */
void uart_init(void);

void uart_deinit(void);


void uart_select(uint8_t uart);

static inline uint8_t uart_RXdata_ready(void);
static inline uint8_t uart_TXdata_ready(void);
static inline uint8_t interUart_RXdata_ready(void);
static inline uint8_t interUart_TXdata_ready(void);

/**
 * Returns True if ring buffer is empty.
 * @return
 */
uint8_t uart_RB_isEmpty(void);

/**
 * Returns True if ring buffer is full.
 * @return
 */
uint8_t uart_RB_isFull(void);

/**
 * Returns the size of ring buffer.
 * @return
 */
uint8_t uart_sizeofRB(void);

/**
 * Reads the ring buffer until no byte left inside and returns a string with the
 * value
 * @return
 */
uint8_t uart_read_RB(char *buffer, uint8_t uart_sel);

/**
 * Clears the ring buffer.
 * @return
 */
void uart_clear_RB(void);

/**
 * Writes a string to the UART using the ring buffer.
 * @param buffer
 */
uint8_t uart_write_RB(char *buffer, uint8_t uart_sel);

/**
 * Returns a byte from the RX rb front.
 * @return
 */
uint8_t uart_pop_RX(void);

/**
 * Returns a byte from the RX rb back.
 * @return
 */
void interUart_init(void);

/**
 * Push a byte  TX rb front.
 * @param byte
 * @return
 */
void uart_push_TX(uint8_t byte);

/**
 * Push a byte to TX rb back.
 * @param byte
 * @return
 */
void uart_pushFront_TX(uint8_t byte);

/**
 * Set the program counter to the BSL location.
 */
void jump_BSL(void);

/**
 * This function is used for the OTA update of the bnvd device.
 * It pass through data from the internal to internal UART and vice versa.
 * It returns when data is no longer transfered for more than 3 seconds.
 * @return
 */
uint8_t uart_pass_through(void);

#endif /* UART_H_ */
