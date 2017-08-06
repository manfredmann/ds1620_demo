/*
 * Copyright 2016 by Roman Serov <roman@serov.in>
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

#ifndef HW_H_INCLUDED
#define HW_H_INCLUDED

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/systick.h>


#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#define RCC_GPIO_LED  RCC_GPIOD
#define GPIO_LED      GPIOD
#define LED_ORANGE    GPIO13
#define LED_GREEN     GPIO12
#define LED_RED       GPIO14
#define LED_BLUE      GPIO15

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

int   _write(int file, char *ptr, int len);
void  _msleep(uint32_t delay);
void  _usleep(uint32_t delay);
void  systick_setup(void);
void  hw_init(void);
void  on_orange_led(void);
void  off_orange_led(void);
void  on_green_led(void);
void  off_green_led(void);
void  on_red_led(void);
void  off_red_led(void);
void  on_blue_led(void);
void  off_blue_led(void);
void  digit_init(void);
void  digit_out(uint8_t data);

#endif