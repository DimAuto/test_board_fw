/**
 * @file max5418.h
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 1.0
 * @date 2022-06-22
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SRC_MAX5418_H_
#define SRC_MAX5418_H_

#include <msp430.h>
#include <stdint.h>
#include "i2c.h"

//Max5418 addresses
#define MAX5418_ADDR            0x2A
#define MAX5418_CTL_VREG        0x11
#define MAX5418_CTL_NVREG       0x21
#define MAX5418_CTL_N_2_VREG    0x61
#define MAX5418_CTL_V_2_NVREG   0x51

typedef enum {          //Enumerator that holds the pot data values that leads to the specific voltage.
    V_2_4   =   128,
    V_2_2   =   116,
    V_1_8   =   89,
    V_1_5   =   69,
    V_1_2   =   50,
    V_0_9   =   32
}Max5418_V_map;

/**
 *  Set the pot to a predefined resistance that leads to a specific VDD.
 *  This function stores the value in the volatile memory of max5418,
 *  so after a power cyrcle the pot value will change to default.
 * @param value The
 * @return The state of the i2c slave.
 */
I2C_Mode max5418_setWiper(Max5418_V_map value);

/**
 * This function sets the default resistance of the max5418 potensiometer, by writing in its non-volatile memory.
 * To change the actual value of the pot the volatile register must also set, or the nvreg get copied to the vreg.
 * @param value
 * @return
 */
I2C_Mode max5418_setDefault(Max5418_V_map value);

#endif /* SRC_MAX5418_H_ */
