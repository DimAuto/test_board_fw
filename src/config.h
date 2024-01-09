/**
 * @file config.h
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 1.0
 * @date 2022-07-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

#include <msp430.h>

#define DEVICE_NAME             "BTB"
#define FW_VERSION              16
//#define NULL                    ((void*)0)
#define UINT16_T_MAX_VALUE      65535
//RINGBUFFER SIZE
#define UART_RB_SIZE            50
//UART FTDI PIN CONFIGURATION
#define UART_FTDI_SEL           P8SEL
#define UART_FTDI_DIR           P8DIR
#define UART_FTDI_OUT           P8OUT
#define UART_FTDI_RX_PIN        BIT3
#define UART_FTDI_TX_PIN        BIT2
#define UART_FTDI_BAUD_RATE     9600

//UART PIN CONFIGURATION
#define UART_SEL                P2SEL
#define UART_DIR                P2DIR
#define UART_OUT                P2OUT
#define UART_RX_PIN             BIT5
#define UART_TX_PIN             BIT4
#define UART_BAUD_RATE          9600

//CLOCK FREQ.
#define CLOCK_FREQUENCY_MHZ     1

//I2C PIN CONFIGURATION
#define I2C_SEL                 P8SEL
#define I2C_DIR                 P8DIR
#define I2C_OUT                 P8OUT
#define I2C_SCL                 BIT6
#define I2C_SDA                 BIT5

//I2C ADDRESSES OF ARM DEVICES
#define MAX542x_L               0x29
#define MAX542x_R               0x28
#define PCA9534_L               0x3C
#define PCA9534_R               0x38
// PCA9534 REGS
#define PCA9534INCOM            0x00    /// PCA9534 input port register command
#define PCA9534OUTCOM           0x01    /// PCA9534 output port register command
#define PCA9534POLCOM           0x02    /// PCA9534 polarity inversion register command
#define PCA9534CONFCOM          0x03    /// PCA9534 configuration register command
#define PCA9534OUTCONF          0x18    /// PCA9534 configuration register data
//MAX941x REGS
#define MAX541XVREG             0x11    /// The MAX541X VREG command
// STATUS_LED PATTERNS
#define STATUS_WORKING          500
#define STATUS_ERROR            200
#define STATUS_IDLE             1000
//ADC CHANNEL-PIN MAPPING
#define bVDD                    0
#define VBAT                    1
#define VSEN                    2
#define MR_B                    3


// PREPROCCESSOR DIRECTIVES
#define BATTERY_INT_EXT_LATCH

#endif /* SRC_CONFIG_H_ */
