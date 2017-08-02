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

#include "ds1620.h"

void ds_gpio_setup(void) {
  rcc_periph_clock_enable(DS_GPIO_RCC);
  
  gpio_mode_setup(DS_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, DS_RST);
  gpio_mode_setup(DS_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, DS_CLK);
  gpio_mode_setup(DS_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, DS_DQ);

  gpio_clear(DS_GPIO, DS_RST);
  gpio_set(DS_GPIO, DS_CLK);
}

void ds_cs(void) {
  on_red_led();
  gpio_set(DS_GPIO, DS_RST);
  _usleep(5);
}

void ds_dcs(void) {
  gpio_set(DS_GPIO, DS_CLK);
  _usleep(5);
  gpio_clear(DS_GPIO, DS_RST);
  off_red_led();
}

void ds_send_byte(uint8_t data) {
  gpio_mode_setup(DS_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, DS_DQ);
  
  uint8_t i;
  
  for (i = 0; i < 8; i++) {
    if (data & (1 << i)) {
      gpio_set(DS_GPIO, DS_DQ);
    } else {
      gpio_clear(DS_GPIO, DS_DQ);
    }
    
    gpio_clear(DS_GPIO, DS_CLK);
    _usleep(5);
    
    gpio_set(DS_GPIO, DS_CLK);
    gpio_set(DS_GPIO, DS_DQ);
    _usleep(5);
  }
}

void ds_send_nine(uint16_t data) {
  gpio_mode_setup(DS_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, DS_DQ);
  
  uint8_t i = 0;
  
    for (i = 0; i < 9; i++) {
    if (data & (1 << i)) {
      gpio_set(DS_GPIO, DS_DQ);
    } else {
      gpio_clear(DS_GPIO, DS_DQ);
    }
    
    gpio_clear(DS_GPIO, DS_CLK);
    _usleep(5);
    
    gpio_set(DS_GPIO, DS_CLK);
    gpio_set(DS_GPIO, DS_DQ);
    _usleep(5);
  }
}

uint8_t ds_read_byte(void) {
  gpio_mode_setup(DS_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, DS_DQ);
  
  uint8_t i = 8;
  uint8_t data = 0;
  
  for (i = 0; i < 8; i++) {
    gpio_set(DS_GPIO, DS_CLK);
    _usleep(5);
    
    gpio_clear(DS_GPIO, DS_CLK);
    _usleep(5);
    
    if (gpio_get(DS_GPIO, DS_DQ)) {
      data |= (1 << i);
    }
  }
  
  return data;
}

uint16_t ds_read_nine(void) {
  gpio_mode_setup(DS_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, DS_DQ);
  
  uint8_t i = 9;
  uint16_t data = 0;
  
  for (i = 0; i < 9; i++) {
    gpio_set(DS_GPIO, DS_CLK);
    _usleep(5);
    
    gpio_clear(DS_GPIO, DS_CLK);
    _usleep(5);
    
    if (gpio_get(DS_GPIO, DS_DQ)) {
      data |= (1 << i);
    }
  }
  return data;
}

uint16_t ds_read_temp(void) {
  ds_cs();

  ds_send_byte(DS_CMD_READ_TMP);
  uint16_t temp = ds_read_nine();
  
  ds_dcs();
  
  return temp;
}

uint8_t ds_read_counter(void) {
  ds_cs();
  
  ds_send_byte(DS_CMD_READ_CNT);
  uint8_t counter = ds_read_byte();
  
  ds_dcs();
  
  return counter;
}

uint8_t ds_read_slope(void) {
  ds_cs();
  
  ds_send_byte(DS_CMD_READ_SLP);
  uint8_t slope = ds_read_byte();
  
  ds_dcs();
  
  return slope;
}

int16_t convert_9bit_16bit(uint16_t data) {
  if ((data & 0x100) == 0) {
    return data;
  } else {
    return (data & 0xFF) | 0xFF00;
  }
}

uint16_t convert_16bit_9bit(int16_t data) {
  if ((data & 0x800) == 0) {
    return data;
  } else {
    return data & 0x1FF;
  }
}

double ds_get_temp(void) {
  uint16_t temp = ds_read_temp();
  int16_t r_temp = convert_9bit_16bit(temp) >> 1; // Strip the LSB
  uint16_t counter = ds_read_counter();
  uint16_t slope = ds_read_slope();
  
  return floor(((r_temp - 0.25) + ((slope - counter) / (double)(slope))) * 10) / 10;  
}

int16_t ds_get_th(void) {
  ds_cs();
  
  ds_send_byte(DS_CMD_READ_TH);
  uint16_t th = ds_read_nine();
  
  ds_dcs();

  return convert_9bit_16bit(th) >> 1;
}

void ds_set_th(int16_t temp) {
  ds_cs();
  
  ds_send_byte(DS_CMD_WRITE_TH);
  ds_send_nine(convert_16bit_9bit(temp << 1));
  
  ds_dcs();
}

int16_t ds_get_tl(void) {
  ds_cs();
  
  ds_send_byte(DS_CMD_READ_TL);
  uint16_t tl = ds_read_nine();
  
  ds_dcs();
  
  return convert_9bit_16bit(tl) >> 1;
}

void ds_set_tl(int16_t temp) {
  ds_cs();
  
  ds_send_byte(DS_CMD_WRITE_TL);
  ds_send_nine(convert_16bit_9bit(temp << 1));
  
  ds_dcs();
}

void ds_start_conv(void) {
  ds_cs();

  ds_send_byte(DS_CMD_START_CNV);

  ds_dcs();
}

void ds_stop_conv(void) {
  ds_cs();

  ds_send_byte(DS_CMD_STOP_CNV);

  ds_dcs();
}

void ds_start_conv_ps(void) {
  gpio_clear(DS_GPIO, DS_RST);
  gpio_set(DS_GPIO, DS_CLK); 
}

void ds_stop_conv_ps(void) {
  gpio_set(DS_GPIO, DS_RST);
  gpio_set(DS_GPIO, DS_CLK); 
}

ds_config ds_read_config(void) {
  ds_cs();
  
  ds_send_byte(DS_CMD_READ_CNF);
  uint8_t config_r = ds_read_byte();
  
  ds_dcs();
  
  return ds_byte_to_conf(config_r);
}

void ds_write_config(ds_config config) {
  
  ds_cs();
  
  ds_send_byte(DS_CMD_WRITE_CNF);
  ds_send_byte(ds_conf_to_byte(config));
  
  ds_dcs();
  
}

ds_config ds_byte_to_conf(uint8_t byte) {
  ds_config config;
  
  config.done     = ((byte & DS_FLAG_DONE) == 0) ? false : true;
  config.thf      = ((byte & DS_FLAG_THF) == 0) ? false : true;
  config.tlf      = ((byte & DS_FLAG_TLF) == 0) ? false : true;
  config.nvb      = ((byte & DS_FLAG_NVB) == 0) ? false : true;
  config.cpu      = ((byte & DS_FLAG_CPU) == 0) ? false : true;
  config.oneshot  = ((byte & DS_FLAG_1SHOT) == 0) ? false : true;
  return config;
}

uint8_t ds_conf_to_byte(ds_config config) {
  uint8_t byte = 0;
  
  if (config.cpu)
    byte |= DS_FLAG_CPU;
    
  if (config.oneshot)
    byte |= DS_FLAG_1SHOT;
  
  if (config.thf)
    byte |= DS_FLAG_THF;
  
  if (config.tlf)
    byte |= DS_FLAG_TLF;
    
  return byte;
}