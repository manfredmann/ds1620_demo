/*
 * Copyright 2017 by Roman Serov <roman@serov.in>
 * 
 * This file is part of DS1620 demo.

 * DS1620 demo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * DS1620 demo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with DS1620 demo. If not, see <http://www.gnu.org/licenses/>.
 * 
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
*/

#include "hw.h"
#include <math.h>
#include <stdlib.h>

#ifndef DIGITS_H_INCLUDED
#define DIGITS_H_INCLUDED

#define RCC_DIGIT_GPIO  RCC_GPIOE
#define DIGIT_GPIO      GPIOE

#define RCC_DIGIT_REG_GPIO      RCC_GPIOA
#define RCC_DIGIT_REG_C2_GPIO   RCC_GPIOC

#define DIGIT_REG_GPIO      GPIOA
#define DIGIT_REG_C2_GPIO   GPIOC

#define DIGIT_REG_DS        GPIO7

#define DIGIT_REG_C2        GPIO4
#define DIGIT_REG_C1        GPIO5

#define DIGIT_D1            GPIO14
#define DIGIT_D2            GPIO13
#define DIGIT_D3            GPIO12
#define DIGIT_D4            GPIO11

#define DIGIT_ZERO          0b11111100
#define DIGIT_ONE           0b01100000
#define DIGIT_TWO           0b11011010
#define DIGIT_THREE         0b11110010
#define DIGIT_FOUR          0b01100110
#define DIGIT_FIVE          0b10110110
#define DIGIT_SIX           0b10111110
#define DIGIT_SEVEN         0b11100000
#define DIGIT_EIGHT         0b11111110
#define DIGIT_NINE          0b11110110

#define DIGIT_DOT_MASK      0b00000001

typedef struct {
  uint8_t values[4];
} digits_data;

void          digits_clear(void);
void          digits_init(void);
static void   digits_out(uint8_t data, bool dot);
void          digits_set(double data);

#endif