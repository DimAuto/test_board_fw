/*
 * internal_uart.h
 *
 *  Created on: Jul 12, 2022
 *      Author: dkalaitzakis
 */

#ifndef SRC_INTERNAL_UART_H_
#define SRC_INTERNAL_UART_H_
//
#include "msp430.h"
#include <stdint.h>

/**
 * Return the boards FW version
 * @return
 */
uint8_t get_board_FWversion(void);

/**
 * Stops the internal uart spamming with acc messages.
 * @return
 */
uint8_t stop_acc_messages(void);

/**
 * Reads the acc messages.
 * @return
 */
uint8_t read_acc(void);

/**
 * Starts the BNVD FW update procedure.
 * The update is based on the <uart_pass_through> function,
 * into the uart module.
 * @return
 */
uint8_t FW_OTA(void);

#endif /* SRC_INTERNAL_UART_H_ */
