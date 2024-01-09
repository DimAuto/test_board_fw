/*
 * gpio_peripherals.c
 *
 *  Created on: Jul 1, 2022
 *      Author: dkalaitzakis
 */

#include "msp430.h"
#include "gpio.h"
#include "gpio_peripherals.h"
#include "adc.h"



Gpio_Pin HLSD, JtagVDD, MR_B;

void gpio_periph_init(void){
    P1DIR = 0x00;
    gpio_initPin(&HLSD, 9, BIT2, OUTPUT);
    gpio_initPin(&JtagVDD, 6, BIT4, OUTPUT);
    gpio_initPin(&MR_B, 6, BIT3, OUTPUT);

    gpio_writePin(&HLSD, LOW);
    gpio_writePin(&JtagVDD, HIGH);
    gpio_writePin(&MR_B, LOW);
}


uint8_t set_HLSD(void){
    return gpio_writePin(&HLSD, HIGH);
}


uint8_t reset_HLSD(void){
    return gpio_writePin(&HLSD, LOW);
}


uint8_t set_Jtag_VDD(void){
    return gpio_writePin(&JtagVDD, LOW);
}


uint8_t reset_Jtag_VDD(void){
    return gpio_writePin(&JtagVDD, HIGH);
}

uint8_t setMR_B(void){
    return gpio_writePin(&MR_B, HIGH);
}


uint8_t resetMR_B(void){
    return gpio_writePin(&MR_B, LOW);
}


float measure_current(void){
    float value = adc_conv(adc_read(2));
    float vout = value * 1.261;
    float iout = vout/(0.13 * 50);

    return iout;
}
