/*
 * helpers.c
 *
 *  Created on: Jun 7, 2022
 *      Author: dkalaitzakis
 */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include <helpers.h>


const char *float_to_char(float num){
    char *tmp;
    char *sign = (num < 0) ? "-" : "";
    float val = (num < 0) ? -num : num;

    int tmp_int1 = val;
    if (val < 1){
        tmp_int1 = 0;
    }
    float tmp_frac = val - tmp_int1;      // Get fraction.
    int tmp_int2 = tmp_frac * 10000;  // Turn into integer.

    sprintf(tmp, "%d.%d", tmp_int1, tmp_int2);

    return tmp;
}


const char *int_to_char(uint16_t num){
    char tmp[16];
    sprintf(tmp, '%d', num);
    return tmp;
}


uint16_t itoa(int value, char *sp, int radix)
{
    char tmp[16];
    char *tp = tmp;
    int i;
    unsigned v;

    uint8_t sign = (radix == 10 && value < 0);
    if (sign)
        v = -value;
    else
        v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix;
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    uint16_t len = tp - tmp;

    if (sign)
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}
