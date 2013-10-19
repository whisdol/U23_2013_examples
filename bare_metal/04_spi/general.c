#include <stm32f4xx.h>

#define wait_latch 10000000

uint32_t convert_color(uint32_t r, uint32_t g, uint32_t b);
void wait_own(int v);
void init_leds(void);
void latch(void);


uint32_t convert_color(uint32_t r, uint32_t g, uint32_t b)
{
  if (r >= 1024) r = 1023;
  if (g >= 1024) g = 1023;
  if (b >= 1024) b = 1023;
  return (g<<0) | (r<<10) | (b<<20);
}

void wait_own(int v)
{
  for (int i=0; i<v; i++);
}



// LEDs
void init_leds(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  GPIO_Init(GPIOD, &(GPIO_InitTypeDef){
      .GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,
      .GPIO_Mode = GPIO_Mode_OUT,
      //.GPIO_OType = GPIO_OType_PP;
      .GPIO_PuPd = GPIO_PuPd_UP,
      .GPIO_Speed = GPIO_Speed_50MHz,
    });
}



// GENERAL
void latch(void)
{
  GPIO_WriteBit(GPIOD, GPIO_Pin_12, SET);
  GPIO_WriteBit(GPIOB, GPIO_Pin_12, SET);
  wait_own(wait_latch);
  GPIO_WriteBit(GPIOD, GPIO_Pin_12, RESET);
  GPIO_WriteBit(GPIOB, GPIO_Pin_12, RESET);
  wait_own(wait_latch);
}
