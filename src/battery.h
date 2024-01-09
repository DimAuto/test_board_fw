/*
 * battery.h
 *
 *  Created on: Jun 20, 2022
 *      Author: dkalaitzakis
 */

#ifndef SRC_BATTERY_H_
#define SRC_BATTERY_H_

#include "max5418.h"


#define MAIN_BATT_OFF   0
#define MAIN_BATT_POL1  1
#define MAIN_BATT_POL2  2

/**
 * Initializes the battery polarity control pins. Default condition is battery disconnected.
 */
void battery_init(void);

/**
 * Sets the battery polarity between 3 options.
 * @param polarity 1)MAIN_BATT_OFF 2)MAIN_BATT_POL1 3)MAIN_BATT_POL2
 * @return 0 if successful
 */
uint8_t battery_set_polarity(uint8_t polarity);

/**
 * Disconnects the battery supply from the polarity switch.
 * @return
 */
uint8_t battery_off(void);

/**
 * Connects the battery supply to the polarity switch.
 * @return
 */
uint8_t battery_on(void);

/**
 * Connects the external battery
 * @return
 */
uint8_t battery_ext_on(void);

/**
 * Disconnects the external battery
 * @return
 */
uint8_t battery_ext_off(void);
/**
 * Sets the battery voltage using the digital pot.
 * @param voltage
 * @return 0 if successful
 */
uint8_t battery_set_voltage(Max5418_V_map voltage);

/**
 * Sets the default voltage of the battery using the digitals pot non-volatile memory.
 * The command will take effect after each power cycle.
 * @param voltage
 * @return
 */
uint8_t battery_set_defaultVoltage(Max5418_V_map voltage);

#endif /* SRC_BATTERY_H_ */
