/**
 * @file message_handler.h
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 0.1
 * @date 2022-06-14
 *
 * A simple module to handle and create fixed form messages that will be useful
 * in any type of communication with other devices.
 *
 *
 */

#ifndef SRC_MESSAGE_HANDLER_H_
#define SRC_MESSAGE_HANDLER_H_

#include <stdint.h>
#include "config.h"

// MESSAGE FIELD LENGTHS.
#define HEADER_L        4
#define PERIPH_TYPE_L   4
#define PERIPH_CMD_L    4

typedef struct {
  char  m_buffer[UART_RB_SIZE];
  char  header[HEADER_L];  // Refers to the type of the message.
                  //e.x. CMD, RES, CTL
  char perph_type[PERIPH_TYPE_L];  // The peripheral type to which the message is
                              // addressed.e.x. GPIO,UART,ADC
  char perph_cmd[PERIPH_CMD_L];   // The command to the specific peripheral e.x.
                              // WRITE/READ/SET/RESET
  uint16_t args;        // In case of CMD message this field holds the
                        // arguments. In case of Response message the result.
  uint16_t kwargs;
  uint8_t m_size;  // The size of the message in bytes.
} message_t;

/**
 * Inits the message handler.
 * @param mes
 */
void m_handler_init(message_t *msg);

/**
 * This function parses a message string and stores it to the message struct.
 * @param message  The message array from the receiver.
 * @return 0 if successful
 */
uint8_t parse_message(message_t *msg);

/**
 * This function takes the message struct as argument concatenates the fields
 * and stores it in the message buffer, to be sent from the transmitter.
 * @param message The message struct that contains the necessary fields to
 * construct a char type message.
 * @return 0 if successful.
 */
static void create_message(message_t* msg);

/**
 *
 */
void clean_message_struct(message_t* msg);

/**
 * The message_handler module interface. It parses the message extract the information and send the proper response.
 * @param msg
 * @return 0 if successful / 1 if the incoming message has errors. / 2 if a uart trans error occurs.
 */
uint8_t process_message(message_t *msg);

/**
 *
 * @param msg
 * @param response
 * @return
 */
static void send_response(message_t *msg, uint16_t response_args);

#endif /* SRC_MESSAGE_HANDLER_H_ */
