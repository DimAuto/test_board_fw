/**
 * @file encoder.h
 * @author Dimitris Kalaitzakis (dkalaitzakis@theon.com)
 * @brief
 * @version 1.0
 * @date 2022-06-03
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SRC_ENCODER_H_
#define SRC_ENCODER_H_

#include <stdint.h>

typedef enum {
    PeriodMS_ULow        = 400,
    dPhaseMs_ULow        = 80,
    pulse_rel_ULow       = 40,
}Enc_speedULow;

typedef enum {
    PeriodMS_Low         = 200,
    dPhaseMs_Low         = 40,
    pulse_rel_Low        = 10,
}Enc_speedLow;

typedef enum {
    PeriodMS_Med         = 22,
    dPhaseMs_Med         = 10,
    pulse_rel_Med        = 3,
}Enc_speedMed;

typedef enum {
    PeriodMS_High        = 12,
    dPhaseMs_High        = 5,
    pulse_rel_High       = 2,
}Enc_speedHigh;

typedef enum {
    PeriodMS_UHigh        = 5,
    dPhaseMs_UHigh        = 2,
    pulse_rel_UHigh       = 1,
}Enc_speedUHigh;

typedef enum {
    Ulow_speed,
    Low_speed,
    Med_speed,
    High_speed,
    Uhigh_speed
}Speed_select;

typedef struct {
    uint16_t periodMs;
    uint16_t d_phaseMs;
    uint16_t releaseMs;
}Enc_speed_vals;

/**
 * Emulates the encoder CW rotation. User can select between 5 predifined speed values.
 * @param positions
 * @param speed
 * @return
 */
uint8_t enc_rotateCW(uint8_t positions, Speed_select speed);

/**
 * Emulates the encoder CCW rotation. User can select between 5 predifined speed values.
 * @param positions
 * @param speed
 * @return
 */
uint8_t enc_rotateCCW(uint8_t positions, Speed_select speed);

/**
 * Emulates the encoder press event.
 * @param press_time_ms
 * @return
 */
uint8_t enc_press(uint16_t press_time_ms);

/**
 * Emulates encoder double click event.
 * @return
 */
uint8_t enc_double_click(uint16_t duration);

/**
 * Initializes the encoder values.
 */
void enc_init(void);

/**
 * Encoder rotate - click - rotate.
 * @return
 */
uint8_t enc_rot_click_rot(void);

/**
 * Emulates the encoders push button press and hold. ! Must be followed by the -enc_release- function.!
 * @return
 */
uint8_t enc_press_hold(void);

/**
 * Releases the encoder's push button. !Must be called after the -enc_press_hold- !
 * @return
 */
uint8_t enc_release(void);

/**
 * Helper static function to set the speed values
 * @param speed
 */
static void setter(Speed_select speed, Enc_speed_vals speed_vals);

#endif /* SRC_ENCODER_H_ */
