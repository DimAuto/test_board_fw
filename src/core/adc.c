/*
 * adc.c
 *
 *  Created on: Jun 6, 2022
 *      Author: dkalaitzakis
 */


#include <stdint.h>
#include <stdio.h>
#include "adc.h"
#include "timer.h"
#include "msp430.h"

ADC_val_t adc_values;

volatile uint16_t l;

uint8_t adc_init(void) {
  // Configure ADC Pins
  P6SEL |= (BIT0 | BIT1 | BIT2);// | BIT3);

  adc_values.adc_val0 = 0;
  adc_values.adc_val1 = 0;
  adc_values.adc_val2 = 0;
  //adc_values.adc_val3 = 0;
  // Configure ADC12
  //  Turn on ADC and enable multiple conversions
  ADC12CTL0 = ADC12ON + ADC12MSC + ADC12SHT0_14;
  // Sampling timer, single sequence
  ADC12CTL1 |= ADC12SHP + ADC12CONSEQ_1;
  ADC12MCTL0 = ADC12INCH_0;
  ADC12MCTL1 = ADC12INCH_1;
  ADC12MCTL2 = ADC12INCH_2 + ADC12EOS; //+END of sequence
  //ADC12MCTL3 = ADC12INCH_3 + ADC12EOS;
  //delayMS(100);
  // Enable ADC interrupt on MEM1
  ADC12IE |= ADC12IE2;

  return 0;
}

void adc_conv_start(void) {
  ADC12IE |= ADC12IE2;
  ADC12CTL0 |= ADC12ENC | ADC12SC;
}

void adc_conv_stop(void) {
  ADC12CTL0 &= ~(ADC12ENC | ADC12SC);
  ADC12IE &= ~ADC12IE2;
}

uint16_t adc_read(uint8_t channel) {
  uint16_t sum;
  uint8_t i;
  // Block until the final channel is loaded with value.
  for (i = 0; i < FILTER_LEN; i++) {
    adc_conv_start();
    delayMS(30);
    while (ADC12IFG & ADC12IFG2) {
      ;
    }
    switch (channel) {
      case 0:
        adc_values.filter_buffer[i] = adc_values.adc_val0;
        break;

      case 1:
        adc_values.filter_buffer[i] = adc_values.adc_val1;
        break;

      case 2:
        adc_values.filter_buffer[i] = adc_values.adc_val2;
        break;
//
//      case 3:
//        adc_values.filter_buffer[i] = adc_values.adc_val3;
//        break;
    }
    adc_conv_stop();
    delayMS(20);
  }

  for (i = 0; i < FILTER_LEN; i++) {
    sum += adc_values.filter_buffer[i];
  }
  return sum / FILTER_LEN;
}

float adc_conv(uint16_t adc_result) {
  float result = (adc_result * (VREF / ADC12RES));
  return result;
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_A_ISR(void) {
  switch (__even_in_range(ADC12IV, 12)) {
    case 0:
      break;  // Vector  0:  No interrupt
    case 2:
      break;  // Vector  2:  ADC overflow
    case 4:
      break;  // Vector  4:  ADC timing overflow
    case 6:   // Vector  6:  ADC12IFG0
      break;
    case 8:  // Vector  8:  ADC12IFG1
      break;
    case 10:  // Vector 10:  ADC12IFG2
      ADC12CTL0 &= ~ADC12SC;
      adc_values.adc_val0 = ADC12MEM0;
      adc_values.adc_val1 = ADC12MEM1;
      adc_values.adc_val2 = ADC12MEM2;
      break;
    case 12:  // Vector 12:  ADC12IFG3
//      ADC12CTL0 &= ~ADC12SC;
//      adc_values.adc_val0 = ADC12MEM0;
//      adc_values.adc_val1 = ADC12MEM1;
//      adc_values.adc_val2 = ADC12MEM2;
     // adc_values.adc_val3 = ADC12MEM3;
      __bic_SR_register_on_exit(LPM0_bits);
      break;

    default:
      break;
  }
}
