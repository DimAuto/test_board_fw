/**
 * @file battery.c
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 1.0
 * @date 2022-07-03
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "battery.h"
#include "timer.h"
#include "gpio.h"
#include "led.h"
#include "i2c.h"
#include "config.h"

Gpio_Pin pol_1_H;
Gpio_Pin pol_1_L;
Gpio_Pin pol_2_H;
Gpio_Pin pol_2_L;

Gpio_Pin gate_ctl_vdd;
Gpio_Pin gate_ctl_vext;

void battery_init(void){
    //initialize all battery related pins as outputs.
    gpio_initPin(&pol_1_H, 3, BIT4, OUTPUT);
    gpio_initPin(&pol_1_L, 3, BIT6, OUTPUT);
    gpio_initPin(&pol_2_H, 3, BIT5, OUTPUT);
    gpio_initPin(&pol_2_L, 3, BIT7, OUTPUT);
    gpio_initPin(&gate_ctl_vdd, 3, BIT3, OUTPUT);
    gpio_initPin(&gate_ctl_vext, 3, BIT2, OUTPUT);

    battery_off();
    battery_ext_off();
    battery_set_polarity(MAIN_BATT_POL1);
    battery_set_voltage(V_1_5);
}

uint8_t battery_set_polarity(uint8_t polarity){
    battery_off();  // isolate the polarity switch from the main circuit before the transaction.
    delayMS(100);
    switch(polarity){
    case 0:
        gpio_writePin(&pol_1_H, HIGH);
        gpio_writePin(&pol_1_L, LOW);
        gpio_writePin(&pol_2_H, HIGH);
        gpio_writePin(&pol_2_L, LOW);
        break;
    case 1:
        gpio_writePin(&pol_1_H, LOW);
        gpio_writePin(&pol_1_L, HIGH);
        gpio_writePin(&pol_2_H, HIGH);
        gpio_writePin(&pol_2_L, LOW);
        break;
    case 2:
        gpio_writePin(&pol_1_H, HIGH);
        gpio_writePin(&pol_1_L, LOW);
        gpio_writePin(&pol_2_H, LOW);
        gpio_writePin(&pol_2_L, HIGH);
        break;
    }
    delayMS(100);
    battery_on();  // re-attach the polarity switch to the main circuit.
    return 0;
}

uint8_t battery_off(void){
    gpio_writePin(&gate_ctl_vdd, HIGH);  // detach the polarity switch from the main circuit.
    return 0;
}

uint8_t battery_on(void){
    gpio_writePin(&gate_ctl_vdd, LOW);  // re-attach the polarity switch to the main circuit.
#ifdef BATTERY_INT_EXT_LATCH
    gpio_writePin(&gate_ctl_vext, LOW);
#endif
    return 0;
}

uint8_t battery_ext_off(void){
    gpio_writePin(&gate_ctl_vext, LOW);  // detach the polarity switch from the main circuit.
    return 0;
}

uint8_t battery_ext_on(void){
    gpio_writePin(&gate_ctl_vext, HIGH);  // re-attach the polarity switch to the main circuit.
#ifdef BATTERY_INT_EXT_LATCH
   gpio_writePin(&gate_ctl_vdd,HIGH);
#endif
    return 0;
}

uint8_t battery_set_voltage(Max5418_V_map voltage){
    I2C_Mode res;
    res = max5418_setWiper(voltage);
    return res;
}

uint8_t battery_set_defaultVoltage(Max5418_V_map voltage){
    I2C_Mode res;
    res = max5418_setDefault(voltage);
    return res;
}

