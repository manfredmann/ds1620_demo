#ifndef DS1620_H_INCLUDED
#define DS1620_H_INCLUDED

#include "hw.h"
#include <math.h>

//GPIO
#define DS_GPIO_RCC RCC_GPIOB
#define DS_GPIO GPIOB
#define DS_RST GPIO12
#define DS_CLK GPIO13
#define DS_DQ  GPIO11

// DS1620 Commands
#define DS_CMD_READ_TMP  0xAA
#define DS_CMD_READ_CNT  0xA0
#define DS_CMD_READ_SLP  0xA9
#define DS_CMD_START_CNV 0xEE
#define DS_CMD_STOP_CNV  0x22
#define DS_CMD_WRITE_TH  0x01
#define DS_CMD_WRITE_TL  0x02
#define DS_CMD_READ_TH   0xA1
#define DS_CMD_READ_TL   0xA2
#define DS_CMD_WRITE_CNF 0x0C
#define DS_CMD_READ_CNF  0xAC

// CONFIG/STATUS Register flags
#define DS_FLAG_DONE  0x80
#define DS_FLAG_THF   0x40
#define DS_FLAG_TLF   0x20
#define DS_FLAG_NVB   0x10
#define DS_FLAG_CPU   0x02
#define DS_FLAG_1SHOT 0x01

//CONFIG/STATUS Register structure
typedef struct {
  bool done;
  bool thf;
  bool tlf;
  bool nvb;
  bool cpu;
  bool oneshot;
} ds_config;

void      ds_gpio_setup(void);

void      ds_send_byte(uint8_t data);
void      ds_send_nine(uint16_t data);
uint8_t   ds_read_byte(void);
uint16_t  ds_read_nine(void);

void      ds_cs(void);
void      ds_dcs(void);
void      ds_start_conv(void);
void      ds_stop_conv(void);

uint16_t  ds_read_temp(void);
uint8_t   ds_read_counter(void);
uint8_t   ds_read_slope(void);
ds_config ds_read_config(void);
void      ds_write_config(ds_config config);
double    ds_get_temp(void);
int16_t   ds_get_th(void);
void      ds_set_th(int16_t th);
int16_t   ds_get_tl(void);
void      ds_set_tl(int16_t tl);

ds_config ds_byte_to_conf(uint8_t byte);
uint8_t   ds_conf_to_byte(ds_config config);

int16_t   convert_9bit_16bit(uint16_t data);
uint16_t  convert_16bit_9bit(int16_t data);

#endif