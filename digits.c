#include "digits.h"

static uint8_t digit = 0;
static digits_data d_data;

void digits_init(void) {
  digit = 0;

  digits_clear();

  rcc_periph_clock_enable(RCC_DIGIT_GPIO);
  rcc_periph_clock_enable(RCC_DIGIT_REG_GPIO);
  rcc_periph_clock_enable(RCC_DIGIT_REG_C2_GPIO);

  gpio_mode_setup(DIGIT_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, DIGIT_D1);
  gpio_mode_setup(DIGIT_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, DIGIT_D2);
  gpio_mode_setup(DIGIT_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, DIGIT_D3);
  gpio_mode_setup(DIGIT_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, DIGIT_D4);

  gpio_mode_setup(DIGIT_REG_C2_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, DIGIT_REG_C2);

  gpio_mode_setup(DIGIT_REG_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, DIGIT_REG_DS);
  gpio_mode_setup(DIGIT_REG_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, DIGIT_REG_C1);

  rcc_periph_clock_enable(RCC_TIM2);
  nvic_enable_irq(NVIC_TIM2_IRQ);
  timer_reset(TIM2);  

  timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  
  timer_set_prescaler(TIM2, 7);

  timer_continuous_mode(TIM2);

  int v = 59999;
  timer_set_period(TIM2, v);
  timer_set_oc_value(TIM2, TIM_OC1, v / 2);

  //timer_disable_oc_clear(TIM2, TIM_OC1);
  //timer_enable_oc_preload(TIM2, TIM_OC1);
  timer_set_oc_slow_mode(TIM2, TIM_OC1);

  timer_enable_counter(TIM2);
  timer_enable_irq(TIM2, TIM_DIER_UIE);
}

static void digits_out(uint8_t data, bool dot) {
  switch(data) {
    case 0: data = DIGIT_ZERO; break;
    case 1: data = DIGIT_ONE; break;
    case 2: data = DIGIT_TWO; break;
    case 3: data = DIGIT_THREE; break;
    case 4: data = DIGIT_FOUR; break;
    case 5: data = DIGIT_FIVE; break;
    case 6: data = DIGIT_SIX; break;
    case 7: data = DIGIT_SEVEN; break;
    case 8: data = DIGIT_EIGHT; break;
    case 9: data = DIGIT_NINE; break;
  }

  if (dot)
    data |= DIGIT_DOT_MASK;

  gpio_clear(DIGIT_REG_C2_GPIO, DIGIT_REG_C2);

  for (uint8_t i = 0; i < 8; i++) {
    if (!(data & (1 << i))) {
      gpio_set(DIGIT_REG_GPIO, DIGIT_REG_DS);
    } else {
      gpio_clear(DIGIT_REG_GPIO, DIGIT_REG_DS);
    }

    gpio_clear(DIGIT_REG_GPIO, DIGIT_REG_C1);
    asm("nop");

    gpio_set(DIGIT_REG_GPIO, DIGIT_REG_C1);
    asm("nop");

    if (i == 7) {
      gpio_set(DIGIT_REG_C2_GPIO, DIGIT_REG_C2);
      asm("nop");
      gpio_clear(DIGIT_REG_C2_GPIO, DIGIT_REG_C2);
    }
  }
}

void digits_clear(void) {
  for (uint8_t i = 0; i < 4; i++) {
    d_data.values[i] = DIGIT_ZERO;
  }
}

void digits_set(double data) {
  uint8_t d_count = 0;

  if (data < 10.0) {
    d_count = 1;
  } else if (data < 100.0) {
    d_count = 2;
  } else if (data < 1000.0) {
    d_count = 3;
  } else {
    return;
  }

  double intp;
  int fractp = abs((float) (modf(data, &intp) * 10));

  d_data.values[0] = fractp;

  for (uint8_t i = 0; i < d_count; i++) {
    int d = abs((float) (modf(((int) data) / pow(10, i + 1), &intp) * 10));
    d_data.values[i+1] = d;
  }
}

void tim2_isr(void) {
  if (timer_get_flag(TIM2, TIM_SR_UIF)) {
    timer_clear_flag(TIM2, TIM_SR_UIF);
    switch(digit) {
      case 0: {
        digits_out(d_data.values[0], false);
        gpio_set(DIGIT_GPIO, DIGIT_D1);
        gpio_clear(DIGIT_GPIO, DIGIT_D2);
        gpio_clear(DIGIT_GPIO, DIGIT_D3);
        gpio_clear(DIGIT_GPIO, DIGIT_D4);
        digit = 1;
        break;
      }
      case 1: {
        digits_out(d_data.values[1], true);
        gpio_clear(DIGIT_GPIO, DIGIT_D1);
        gpio_set(DIGIT_GPIO, DIGIT_D2);
        gpio_clear(DIGIT_GPIO, DIGIT_D3);
        gpio_clear(DIGIT_GPIO, DIGIT_D4);
        digit = 2;
        break;
      }
      case 2: {
        digits_out(d_data.values[2], false);
        gpio_clear(DIGIT_GPIO, DIGIT_D1);
        gpio_clear(DIGIT_GPIO, DIGIT_D2);
        gpio_set(DIGIT_GPIO, DIGIT_D3);
        gpio_clear(DIGIT_GPIO, DIGIT_D4);
        digit = 3;
        break;
      }
      case 3: {
        digits_out(d_data.values[3], false);
        gpio_clear(DIGIT_GPIO, DIGIT_D1);
        gpio_clear(DIGIT_GPIO, DIGIT_D2);
        gpio_clear(DIGIT_GPIO, DIGIT_D3);
        gpio_set(DIGIT_GPIO, DIGIT_D4);
        digit = 0;
        break;
      }
    }
  }
}