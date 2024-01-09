/**
 * @file message_handler.c
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 0.1
 * @date 2022-06-14
 *
 *
 *
 */

#include <stdlib.h>
#include <string.h>

#include "message_handler.h"
#include "uart.h"
#include "timer.h"
#include "adc.h"
#include "led.h"
#include "battery.h"
#include "encoder.h"
#include "config.h"
#include "test_board_leds.h"
#include "helpers.h"
#include "internal_uart.h"
#include "gpio_peripherals.h"

volatile uint8_t bus_ready_flag = 0;
extern led_t status_led;

void m_handler_init(message_t* msg) {
  msg->m_size = UART_RB_SIZE;
  msg->args = 0;
  msg->kwargs = 0;
  memset(msg->m_buffer, 0, msg->m_size);
  memset(msg->header, 0, HEADER_L);
  memset(msg->perph_type, 0,PERIPH_TYPE_L);
  memset(msg->perph_cmd, 0, PERIPH_CMD_L);
}

uint8_t parse_message(message_t * msg) {
  const char delimiter[2] = ":";  // Splits the string on every "+" char.
  char * token = strtok(msg->m_buffer, delimiter);

  uint8_t i = 0;
  for (i = 0; token != NULL; i++) {
//    token[strcspn(token, "\n")] = "\0";
    switch (i) {
      case 0:
        memcpy(msg->header, token, HEADER_L);
        break;
      case 1:
        memcpy(msg->perph_type, token, PERIPH_TYPE_L);
        break;
      case 2:
        memcpy(msg->perph_cmd, token, PERIPH_CMD_L);
        break;
      case 3:
        msg->args = atoi(token);
        //memcpy(msg->args, token, ARGS_L);
        break;
      case 4:
        msg->kwargs = atoi(token);
        break;
      case 5:
        clean_message_struct(msg);
        return 0;
    }
    token = strtok(NULL, delimiter);  // Next token
  }
  if (i<3){ //If the number of fields parsed is less than 3. Return error.
      clean_message_struct(msg);
      return 0;
  }
  return 1;
}

static void create_message(message_t* msg) {
  char *args;
  char buffer[UART_RB_SIZE] = {'\0'};

  itoa(msg->args, args, 10);
  strcpy(buffer, "#");
  strcat(buffer, msg->header);
  strcat(buffer, ":");
  strcat(buffer, msg->perph_type);
  strcat(buffer, ":");
  strcat(buffer, msg->perph_cmd);
  strcat(buffer, ":");
  strcat(buffer, args);
  strcat(buffer, "\r");
  memcpy(msg->m_buffer, buffer, UART_RB_SIZE);

}

void clean_message_struct(message_t* msg){
    memset(msg->m_buffer, 0, msg->m_size);
    memset(msg->header, 0, HEADER_L);
    memset(msg->perph_type, 0,PERIPH_TYPE_L);
    memset(msg->perph_cmd, 0, PERIPH_CMD_L);
    msg->args = 0;
}

uint8_t process_message(message_t* msg) {
    uint16_t response_args = 0;

    if (!strcmp(msg->header, "CMD")){
        if (!bus_ready_flag) goto transm_init_request;  //If bus not ready request re-initialization

        if (!strcmp(msg->perph_type, "ADC")){
            if(!strcmp(msg->perph_cmd, "R_D")){         //READ
                if (msg->args < 0 || msg->args > 2) goto msg_error;
                response_args = adc_read(msg->args);
            }
            else goto msg_error;
        }
        else if (!strcmp(msg->perph_type, "HLS")){      //HLSD
            if(!strcmp(msg->perph_cmd, "SET")){         //Set
                response_args = set_HLSD();
            }
            else if (!strcmp(msg->perph_cmd, "RST")){   //Reset
                response_args = reset_HLSD();
            }
            else goto msg_error;
        }
        else if (!strcmp(msg->perph_type, "JDD")){      //Jtag_vdd
            if(!strcmp(msg->perph_cmd, "SET")){         //Set
                response_args = set_Jtag_VDD();
            }
            else if (!strcmp(msg->perph_cmd, "RST")){   //Reset
                response_args = reset_Jtag_VDD();
            }
            else goto msg_error;
        }
        else if (!strcmp(msg->perph_type, "MRB")){      //MR_B
            if(!strcmp(msg->perph_cmd, "SET")){         //Set
                response_args = setMR_B();
            }
            else if (!strcmp(msg->perph_cmd, "RST")){   //Reset
                response_args = resetMR_B();
            }
            else goto msg_error;
        }
        else if (!strcmp(msg->perph_type, "BAT")){      //Battery
            if(!strcmp(msg->perph_cmd, "ON_")){         //ON
                if (msg->args == 1) response_args = battery_on();
                else if (msg->args == 2) response_args = battery_ext_on();
                else goto msg_error;
            }
            else if (!strcmp(msg->perph_cmd, "OFF")){   //OFF
                if (msg->args == 1) response_args = battery_off();
                else if (msg->args == 2) response_args = battery_ext_off();
                else goto msg_error;
            }
            else if (!strcmp(msg->perph_cmd, "SET")){   //SET_VOLTAGE
                if ((msg->args < V_0_9) || (msg->args > V_2_4)) goto msg_error;
                response_args = battery_set_voltage(msg->args);
            }
            else goto msg_error;
        }
        else if (!strcmp(msg->perph_type, "ENC")){      //Encoder
            uint8_t positions = msg->args;
            uint8_t speed     = msg->kwargs;
            if(!strcmp(msg->perph_cmd, "RCW")){         //Rotate Clockwise
                response_args = enc_rotateCW(positions, speed);
            }
            else if (!strcmp(msg->perph_cmd, "RCC")){   //Rotate Counterclockwise
                response_args = enc_rotateCCW(positions, speed);
            }
            else if (!strcmp(msg->perph_cmd, "PRS")){   //Press
                response_args = enc_press(msg->args);
            }
            else if (!strcmp(msg->perph_cmd, "DBC")){   //Press
                response_args = enc_double_click(msg->args);
            }
            else if (!strcmp(msg->perph_cmd, "RCR")){   //Press
                response_args = enc_rot_click_rot();
            }
            else if (!strcmp(msg->perph_cmd, "PHO")){   //Press
                response_args = enc_press_hold();
            }
            else if (!strcmp(msg->perph_cmd, "PRE")){   //Press
                response_args = enc_release();
            }
            else goto msg_error;
        }
        else if (!strcmp(msg->perph_type, "I2C")){
            if(!strcmp(msg->perph_cmd, "LIS")){         // Listen at specific peripheral address.
               response_args = I2C_Slave_Listen(msg->args, msg->kwargs);
            }
            else if (!strcmp(msg->perph_cmd, "RDR")){   // Read slave register command
                response_args = I2C_Slave_ReadReg();
            }
            else if (!strcmp(msg->perph_cmd, "RDD")){   // Read slave data command
                response_args = I2C_Slave_ReadData();
            }
            else if (!strcmp(msg->perph_cmd, "SMD")){   // Set mock tx data
                response_args = I2C_Slave_set_mock_data(msg->args, msg->kwargs);
            }
            else goto msg_error;
        }

        else if (!strcmp(msg->perph_type, "SER")){

            if(!strcmp(msg->perph_cmd, "FWV")){
                    response_args = get_board_FWversion();
                }

            else if (!strcmp(msg->perph_cmd, "STS")){
                response_args = stop_acc_messages();
            }
            else if (!strcmp(msg->perph_cmd, "RAC")){
                response_args = read_acc();
            }
            else if (!strcmp(msg->perph_cmd, "BSL")){
                jump_BSL();
            }
            else if (!strcmp(msg->perph_cmd, "OTA")){
                response_args = FW_OTA();
            }
            else goto msg_error;
        }
        else if (!strcmp(msg->perph_type, "LED")){
            if(!strcmp(msg->perph_cmd, "SET")){         //SET
                response_args = TB_set_led(msg->args);
            }
            else if (!strcmp(msg->perph_cmd, "RST")){   //RESET
                response_args = TB_reset_led(msg->args);
            }
            else if (!strcmp(msg->perph_cmd, "MST")){   //Multiple Set
                response_args = TB_multiple_led_set(msg->args);
            }
            else if (!strcmp(msg->perph_cmd, "RSA")){   //Reset All LEDs
                response_args = TB_reset_all();
            }
            else goto msg_error;

        }
        else goto msg_error;
    }
    else if (!strcmp(msg->header, "CTL")){
        if((!strcmp(msg->perph_cmd, "INI")) && (!bus_ready_flag) ){         //Handshake
            bus_ready_flag = 1;
            memcpy(msg->perph_type, "AKN", PERIPH_TYPE_L);
            memcpy(msg->perph_cmd, DEVICE_NAME, PERIPH_TYPE_L);
            status_led.pattern.delay_ms = STATUS_WORKING;
            response_args = FW_VERSION;
        }
        else if((!strcmp(msg->perph_cmd, "RST")) && (bus_ready_flag) ){         //Reset Communication
            bus_ready_flag = 0;
            status_led.pattern.delay_ms = STATUS_IDLE;
            memcpy(msg->perph_type, "RST", PERIPH_TYPE_L);
            memcpy(msg->perph_cmd, "AKN", PERIPH_TYPE_L);
            response_args = 0;
        }

        else goto msg_error;
    }

    else goto msg_error;

    send_response(msg, response_args);

    return 0;

    msg_error:    // The message has errors, send a Command retransmit request.
        memcpy(msg->perph_cmd, "ERR", PERIPH_CMD_L);
        send_response(msg, 1);  //send response with args=1 as error flag
        return 1;

    transm_init_request:
        memcpy(msg->header, "CTL", HEADER_L);
        memcpy(msg->perph_cmd, "RIN", PERIPH_CMD_L);       //Re-initialize the communication by sending handshake
        status_led.pattern.delay_ms = STATUS_ERROR;
        send_response(msg, 2);
        return 2;
}

static void send_response(message_t *msg, uint16_t response_args){

    memset(msg->m_buffer, 0, msg->m_size);  //Empty the message buffer
    memcpy(msg->header, "RSP", HEADER_L);
    msg->args = response_args;

    create_message(msg);
}

