/*
 * max5419.c
 *
 *  Created on: Jun 22, 2022
 *      Author: dkalaitzakis
 */

#include <src/max5418.h>

#include "battery.h"
#include "timer.h"

I2C_Mode max5418_setWiper(Max5418_V_map value) {
  uint8_t reg_data[1] = {value};
  I2C_Mode tmp = IDLE_MODE;
  tmp = I2C_Master_WriteReg(MAX5418_ADDR, MAX5418_CTL_VREG, reg_data, 1);
  return tmp;
}

I2C_Mode max5418_setDefault(Max5418_V_map value) {
  uint8_t reg_data[1] = {value};
  return I2C_Master_WriteReg(MAX5418_ADDR, MAX5418_CTL_NVREG, reg_data, 1);
}
