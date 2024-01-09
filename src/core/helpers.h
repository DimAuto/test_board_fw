/*
 * helpers.h
 *
 *  Created on: Jun 7, 2022
 *      Author: dkalaitzakis
 */

#ifndef SRC_CORE_HELPERS_H_
#define SRC_CORE_HELPERS_H_

#include <stdint.h>

const char *float_to_char(float num);

const char *int_to_char(uint16_t num);

uint16_t itoa(int value, char *sp, int radix);

#endif /* SRC_CORE_HELPERS_H_ */
