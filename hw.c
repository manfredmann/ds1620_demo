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

#include "hw.h"

volatile uint32_t system_millis;

void sys_tick_handler(void) {
  system_millis++;
}

void _usleep(uint32_t delay) {
  uint32_t wake = system_millis + delay;
  while (wake > system_millis);
}

void _msleep(uint32_t delay) {
  _usleep(delay * 1000);
}

int _write(int file, char *ptr, int len) {
  int i;

  if (file == STDOUT_FILENO || file == STDERR_FILENO) {
    for (i = 0; i < len; i++) {
      if (ptr[i] == '\n') {
        usart_send_blocking(USART2, '\r');
      }
      usart_send_blocking(USART2, ptr[i]);
    }
    return i;
  }
  errno = EIO;
  return -1;
}

void systick_setup(void) {
  systick_set_reload(168);
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  systick_counter_enable();
  systick_interrupt_enable();
}


void hw_init(void) {
  rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
  
  // USART setup
  rcc_periph_clock_enable(RCC_GPIOA);
    
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);
  gpio_set_af(GPIOA, GPIO_AF7, GPIO3);
  gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
  
  rcc_periph_clock_enable(RCC_USART2);
  usart_set_baudrate(USART2, 115200);
  usart_set_databits(USART2, 8);
  usart_set_stopbits(USART2, USART_STOPBITS_1);
  usart_set_mode(USART2, USART_MODE_TX_RX);
  usart_set_parity(USART2, USART_PARITY_NONE);
  usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
  
  usart_enable(USART2);
  
  // LEDs setup
  
  rcc_periph_clock_enable(RCC_GPIO_LED);
  gpio_mode_setup(GPIO_LED, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LED_BLUE);
  gpio_mode_setup(GPIO_LED, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LED_GREEN);
  gpio_mode_setup(GPIO_LED, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LED_ORANGE);
  gpio_mode_setup(GPIO_LED, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LED_RED);
  
  off_blue_led();
  off_green_led();
  off_orange_led();
  off_red_led();
  
  // Systick
  systick_setup();
}

void on_blue_led(void) {
  gpio_set(GPIO_LED, LED_BLUE);
}

void off_blue_led(void) {
  gpio_clear(GPIO_LED, LED_BLUE);
}

void on_green_led(void) {
  gpio_set(GPIO_LED, LED_GREEN);
}

void off_green_led(void) {
  gpio_clear(GPIO_LED, LED_GREEN);
}

void on_orange_led(void) {
  gpio_set(GPIO_LED, LED_ORANGE);
}

void off_orange_led(void) {
  gpio_clear(GPIO_LED, LED_ORANGE);
}

void on_red_led(void) {
  gpio_set(GPIO_LED, LED_RED);
}

void off_red_led(void) {
  gpio_clear(GPIO_LED, LED_RED);
}