/**
 * @file encoder.c
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 1.0
 * @date 2022-06-28
 *
 * @copyright Copyright (c) 2022
 *
 */


#include "encoder.h"
#include "gpio.h"
#include "timer.h"
#include "test_board_leds.h"


Gpio_Pin encB, encA, encSout;


void enc_init(){
    gpio_initPin(&encB, 9, BIT3, OUTPUT);
    gpio_initPin(&encA, 9, BIT5, OUTPUT);
    gpio_initPin(&encSout, 9, BIT4, OUTPUT);

    gpio_writePin(&encB, HIGH);
    gpio_writePin(&encA, HIGH);
    gpio_writePin(&encSout, LOW);
}

static void setter(Speed_select speed, Enc_speed_vals speed_vals){
    switch (speed){
    case Ulow_speed:
        speed_vals.periodMs    = PeriodMS_ULow;
        speed_vals.d_phaseMs   = dPhaseMs_ULow;
        speed_vals.releaseMs   = pulse_rel_ULow;
        break;
    case Low_speed:
        speed_vals.periodMs    = PeriodMS_Low;
        speed_vals.d_phaseMs   = dPhaseMs_Low;
        speed_vals.releaseMs   = pulse_rel_Low;
        break;
    case Med_speed:
        speed_vals.periodMs    = PeriodMS_Med;
        speed_vals.d_phaseMs   = dPhaseMs_Med;
        speed_vals.releaseMs   = pulse_rel_Med;
        break;
    case High_speed:
        speed_vals.periodMs    = PeriodMS_High;
        speed_vals.d_phaseMs   = dPhaseMs_High;
        speed_vals.releaseMs   = pulse_rel_High;
        break;
    case Uhigh_speed:
        speed_vals.periodMs    = PeriodMS_UHigh;
        speed_vals.d_phaseMs   = dPhaseMs_UHigh;
        speed_vals.releaseMs   = pulse_rel_UHigh;
        break;

    default:
        speed_vals.periodMs    = PeriodMS_High;
        speed_vals.d_phaseMs   = dPhaseMs_High;
        speed_vals.releaseMs   = pulse_rel_High;
        break;
    }
}

uint8_t enc_rotateCW(uint8_t positions, Speed_select speed){
    Enc_speed_vals speed_vals = {.d_phaseMs = 0, .periodMs = 0, .releaseMs = 0};
    setter(speed, speed_vals);
    uint8_t i;
    for(i = 0; i<positions; i++){
        gpio_writePin(&encA, LOW);
        delayMS(speed_vals.d_phaseMs);
        gpio_writePin(&encB, LOW);
        delayMS(speed_vals.releaseMs);
        gpio_writePin(&encA, HIGH);
        delayMS(speed_vals.releaseMs);
        gpio_writePin(&encB, HIGH);
        delayMS(speed_vals.periodMs);
    }
    return 0;
}

uint8_t enc_rotateCCW(uint8_t positions, Speed_select speed){
    Enc_speed_vals speed_vals = {.d_phaseMs = 0, .periodMs = 0, .releaseMs = 0};
    setter(speed, speed_vals);
    uint8_t i;
    for(i = 0; i<positions; i++){
        gpio_writePin(&encB, LOW);
        delayMS(speed_vals.d_phaseMs);
        gpio_writePin(&encA, LOW);
        delayMS(speed_vals.releaseMs);
        gpio_writePin(&encB, HIGH);
        delayMS(speed_vals.releaseMs);
        gpio_writePin(&encA, HIGH);
        delayMS(speed_vals.periodMs);
    }
    return 0;
}

uint8_t enc_press(uint16_t press_time_ms){
    gpio_writePin(&encSout, HIGH);
    delayMS(press_time_ms);
    gpio_writePin(&encSout, LOW);
    return 0;
}

uint8_t enc_press_hold(void){
    gpio_writePin(&encSout, HIGH);
    return 0;
}

uint8_t enc_release(void){
    gpio_writePin(&encSout, LOW);
    return 0;
}

uint8_t enc_double_click(uint16_t duration){
    enc_press(duration);
    delayMS(duration);
    enc_press(duration);
    TB_toggle_led(1);
    return 0;
}

uint8_t enc_rot_click_rot(void){
    if (enc_rotateCCW(3, Med_speed)) return 1;
    delayMS(200);
    if (enc_press(100)) return 1;
    delayMS(200);
    if (enc_rotateCCW(3, Med_speed)) return 1;
    return 0;
}


