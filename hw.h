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

#endif