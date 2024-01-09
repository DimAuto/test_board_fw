
/**
 * @file internal_uart.c
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 0.1
 * @date 2022-06-03
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <src/internal_uart.h>
#include "uart.h"
#include "timer.h"
#include "battery.h"

extern volatile uint8_t intTransEnd_flag;
extern uint8_t ota_complete;

uint8_t get_board_FWversion(void){
    uint16_t timeout = 0;
    char info[50] = {"\0"};
    uart_write_RB("$GI.", INTERNAL);
    timeout = millis();
    while (!intTransEnd_flag){
        if (ellapsed_millis(timeout) > 1000){
            intTransEnd_flag = 0;
            return 1;
        }
    }
    uart_read_RB(info, INTERNAL);

    return info[14];
}

uint8_t stop_acc_messages(void){
    char tmp[50] = {"\0"};
    uart_write_RB("$SX.", INTERNAL);
    uart_read_RB(tmp, INTERNAL);    //dummy read
    return 0;
}

uint8_t read_acc(void){
    uint16_t timeout = 0;
    char data[50] = {"\0"};
    intTransEnd_flag = 0;
    timeout = millis();
    uart_read_RB(data, INTERNAL); //dummy read to enable RX interrupt
    while (!intTransEnd_flag){
        if (ellapsed_millis(timeout) > 1000){
            intTransEnd_flag = 0;
            return 1;
        }
    }
    uart_read_RB(data, INTERNAL);
    return data[2];
    }

uint8_t FW_OTA(void){
    uart_write_RB("$SNFU.", INTERNAL);
    delayMS(600);
    uart_pass_through();
    while (!ota_complete){
        ;
    }
    return 0;
}

