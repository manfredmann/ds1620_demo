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
#include "ds1620.h"
#include <stdlib.h>

static char get_char(uint32_t usart) {
  return usart_recv_blocking(usart);
}

static void print_config(ds_config config) {
  printf("DONE: %c",      config.done ? '1' : '0');
  printf(" THF: %c",      config.thf ? '1' : '0');
  printf(" TLF: %c",      config.tlf ? '1' : '0');
  printf(" NVB: %c",      config.nvb ? '1' : '0');
  printf(" CPU: %c",      config.cpu ? '1' : '0');
  printf(" 1SHOT: %c",    config.oneshot ? '1' : '0');
  printf("\n");
}

static void print_registers(ds_config config, int16_t th, int16_t tl) {
  printf("============================== Registers ===============================\n");
  printf("CONFIG\t=\t");
  
  printf("DONE:\t%c\n",       config.done ? '1' : '0');
  printf("\t\tTHF:\t%c\n",    config.thf ? '1' : '0');
  printf("\t\tTLF:\t%c\n",    config.tlf ? '1' : '0');
  printf("\t\tNVB:\t%c\n",    config.nvb ? '1' : '0');
  printf("\t\tCPU:\t%c\n",    config.cpu ? '1' : '0');
  printf("\t\t1SHOT:\t%c\n",  config.oneshot ? '1' : '0');

  printf("TH\t=\t%d c\n", th);
  printf("TL\t=\t%d c\n", tl);
  printf("\n");
}

static void print_menu(void) {
  printf("================================ Menu ==================================\n");
  printf("What can i do?\n");
  printf("0: Set CONFIG register to default\n");
  printf("1: Toggle CPU flag\n");
  printf("2: Toggle 1SHOT flag\n");
  printf("3: Set TH\n");
  printf("4: Set TL\n");
  printf("5: Reset THF\n");
  printf("6: Reset TLF\n");
  printf("7: Continuous mode\n");
  printf("8: One shot mode\n");
  printf("\n");
  printf("Type number, or press any key to read registers again\n");
}

static void write_conf(ds_config config) {
  printf("Current CONFIG =\t");
  print_config(config);
  
  ds_write_config(config);
  
  printf("New CONFIG =\t\t");
  print_config(config);
}

static void write_default_conf(void) {
  ds_config config;
  
  config.cpu      = false;
  config.oneshot  = false;
  config.thf      = false;
  config.tlf      = false;
  
  write_conf(config);
}

static void toggle_conf_cpu(ds_config config) {
  printf("Current CONFIG =\t");
  print_config(config);
  
  config.cpu = !config.cpu;
  
  ds_write_config(config);
  
  printf("New CONFIG =\t\t");
  print_config(config);
}

static void toggle_conf_oneshot(ds_config config) {
  printf("Current CONFIG =\t");
  print_config(config);
  
  config.oneshot = !config.oneshot;
  
  ds_write_config(config);
  
  printf("New CONFIG =\t\t");
  print_config(config);
}

static void reset_thf(ds_config config) {
  printf("Current CONFIG =\t");
  print_config(config);
  
  config.thf = false;
  
  ds_write_config(config);
  
  printf("New CONFIG =\t\t");
  print_config(config);
}

static void reset_tlf(ds_config config) {
  printf("Current CONFIG =\t");
  print_config(config);
  
  config.tlf = false;
  
  ds_write_config(config);
  
  printf("New CONFIG =\t\t");
  print_config(config);
}

static void one_shot_mode(void) {
  printf("\n=========================== One shot mode ==============================\n");
  
  on_orange_led();
  
  ds_start_conv();
  
  ds_config config;
  
  uint8_t count = 0;
  do {
    if (++count == 20) {
      printf("Hmm. Set 1SHOT to 1\n");
      return;
    }
    
    config = ds_read_config();
    printf("CONFIG =\t");
    print_config(config);
    _msleep(100);
  } while (config.done == false);
  
  double temp = ds_get_temp();
  printf("Temp =\t\t%.1f c\n", temp);
  
  ds_stop_conv();
  
  off_orange_led();
}

static void continue_mode(void) {
  printf("\n========================== Continuous mode =============================\n");
  ds_start_conv();
  
  ds_config config;
  
  uint16_t counter = 0;
  
  while (1) {
    config = ds_read_config();
    
    on_green_led();
    
    double temp = ds_get_temp();
    printf("%d:\tTemp = %.1f c\t", counter, temp);
    
    off_green_led();
    
    printf("CONFIG = ");
    print_config(config);

    if (usart_recv(USART2) == ' ')
      break;
    _msleep(750);
    counter++;
  }
  ds_stop_conv();
}

static int16_t input_temp(void) {
  printf("Type degrees in celsius (Example: 10 or -10, etc.). Must be -55 < t < 125 \n");
  
  char c;
  uint8_t max_length = 5;
  uint8_t i = 0;
  char line[max_length];
  
  while ((c = get_char(USART2)) != '\r') {
    if (i == (max_length - 1))
      continue;
    usart_send(USART2, c);
    line[i] = c;
    i++;
  }
  
  line[i] = '\0';
  return atoi(&line[0]);
}

static void setTH(void) {
  printf("\n=============================== Set TH =================================");
  
  int16_t temp;
  
  do {
    printf("\n");
    temp = input_temp();
  } while (temp < -55 || temp > 125);
  
  ds_set_th(temp);
  int16_t th = ds_get_th();
  
  printf("\nTH set to:\t%d c\n", th);
}

static void setTL(void) {
  printf("\n=============================== Set TL =================================");
  
  int16_t temp;
  
  do {
    printf("\n");
    temp = input_temp();
  } while (temp < -55 || temp > 125);
  
  ds_set_tl(temp);
  int16_t th = ds_get_tl();
  
  printf("\nTL set to:\t%d c\n", th);
}

int main(void) {
  hw_init();
  ds_gpio_setup();
  
  _msleep(250);
  
  while (1) {
    usart_send_blocking(USART2, 27);
    printf("[2J");
    usart_send_blocking(USART2, 27);
    
    printf("\n========================================================================\n");
    printf("| DS1620 Demo\t\t\tSTM32F407\t\tRoman G. Serov |\n");
    printf("========================================================================\n\n");
    
    ds_config config = ds_read_config();
    
    print_registers(config, ds_get_th(), ds_get_tl());
    print_menu();
    
    char c = get_char(USART2);
    
    switch (c) {
      case '0': write_default_conf(); break;
      case '1': toggle_conf_cpu(config); break;
      case '2': toggle_conf_oneshot(config); break;
      case '3': setTH(); break;
      case '4': setTL(); break;
      case '5': reset_thf(config); break;
      case '6': reset_tlf(config); break;
      case '7': continue_mode(); break;
      case '8': one_shot_mode(); break;
      default : continue;
    }
    
    printf("Press any key...\n");
    get_char(USART2);
  }
  return 0;
}