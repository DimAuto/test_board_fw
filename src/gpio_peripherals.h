/**
 * @file gpio_peripherals.h
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 0.1
 * @date 2022-07-01
 *
 *
 *
 */

#ifndef SRC_GPIO_PERIPHERALS_H_
#define SRC_GPIO_PERIPHERALS_H_

#include <stdint.h>

/**
 * Inits the peripheral module
 */
void gpio_periph_init(void);

/**
 * Sets the HLSD bit to simulate high light on the photocell.
 * @return
 */
uint8_t set_HLSD(void);

/**
 * Resets the HLSD to simulate low light on the photocell.
 * @return
 */
uint8_t reset_HLSD(void);

/**
 * Sets the jtag vdd so the DUT can be programmed with BSL.
 * @return
 */
uint8_t set_Jtag_VDD(void);

/**
 * Resets the jtag vdd
 * @return
 */
uint8_t reset_Jtag_VDD(void);

/**
 * Sets the magnetic read switch pin to emulate an open switch.(Goggles lifted)
 * @return
 */
uint8_t setMR_B(void);

/**
 * Reset the magnetic read switch pin to emulate a closed switch.(Goggles engaged)
 * @return
 */
uint8_t resetMR_B(void);
/**
 *
 */
float measure_current(void);

#endif /* SRC_GPIO_PERIPHERALS_H_ */
