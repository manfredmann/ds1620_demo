#ifndef HW_H_INCLUDED
#define HW_H_INCLUDED

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
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