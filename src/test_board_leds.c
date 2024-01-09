/**
 * @file test_board_leds.c
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 0.1
 * @date 2022-07-01
 *
 *
 *
 */
#include <stdint.h>
#include "test_board_leds.h"
#include "led.h"

led_t ds1, ds2, ds3, ds4;

void TB_led_init(void){
    led_init(&ds1, 6, BIT5);
    led_init(&ds2, 6, BIT6);
    led_init(&ds3, 6, BIT7);
    led_init(&ds4, 7, BIT4);

    TB_reset_all();     //Initiliaze the module with all leds off.
}

uint8_t TB_set_led(uint8_t led_number){
    if ((led_number < 1) && (led_number > 5)) return 0;
    switch (led_number){
    case 1:
        led_setState(&ds1, LED_ON);
        break;
    case 2:
        led_setState(&ds2, LED_ON);
        break;
    case 3:
        led_setState(&ds3, LED_ON);
        break;
    case 4:
        led_setState(&ds4, LED_ON);
        break;
       }
    return 0;
}

uint8_t TB_toggle_led(uint8_t led_number){
    if ((led_number < 1) && (led_number > 5)) return 0;
    switch (led_number){
    case 1:
        led_toggle(&ds1);
        break;
    case 2:
        led_toggle(&ds2);
        break;
    case 3:
        led_toggle(&ds3);
        break;
    case 4:
        led_toggle(&ds4);
        break;
    }
    return 0;
}

uint8_t TB_reset_led(uint8_t led_number){
    if ((led_number < 1) && (led_number > 5)) return 0;
    switch (led_number){
    case 1:
        led_setState(&ds1, LED_OFF);
        break;
    case 2:
        led_setState(&ds2, LED_OFF);
        break;
    case 3:
        led_setState(&ds3, LED_OFF);
        break;
    case 4:
        led_setState(&ds4, LED_OFF);
        break;
    }
    return 0;
}

uint8_t TB_multiple_led_set(uint8_t led_array){
    if ((led_array < 1) && (led_array > 5)) return 0;
    switch (led_array){
    case 1:
        led_setState(&ds1, LED_ON);
        break;
    case 2:
        led_setState(&ds1, LED_ON);
        led_setState(&ds2, LED_ON);
        break;
    case 3:
        led_setState(&ds1, LED_ON);
        led_setState(&ds2, LED_ON);
        led_setState(&ds3, LED_ON);
        break;
    case 4:
        led_setState(&ds1, LED_ON);
        led_setState(&ds2, LED_ON);
        led_setState(&ds3, LED_ON);
        led_setState(&ds4, LED_ON);
        break;
    }
    return 0;
}

uint8_t TB_reset_all(void){
    led_setState(&ds1, LED_OFF);
    led_setState(&ds2, LED_OFF);
    led_setState(&ds3, LED_OFF);
    led_setState(&ds4, LED_OFF);
    return 0;
}


