#include <stm32f4xx.h>

//#define wait_data 0
#define wait_data 0
void wait_own(int v);

// GPIO SPI
void init_gpiospi(void);
void gpiospi_write_int32(uint32_t data);



// GPIO SPI
void init_gpiospi(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  // SPI  (SCK PB13, MOSI PB15)
  GPIO_Init(GPIOB, &(GPIO_InitTypeDef){
      .GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15,
      .GPIO_Mode = GPIO_Mode_OUT,
      .GPIO_Speed = GPIO_Speed_50MHz,
    });

  // Latch / (Chip Select)
  GPIO_Init(GPIOB, &(GPIO_InitTypeDef){
      .GPIO_Pin = GPIO_Pin_12,
      .GPIO_Mode = GPIO_Mode_OUT,
      .GPIO_Speed = GPIO_Speed_50MHz,
    });

  GPIO_WriteBit(GPIOB, GPIO_Pin_12, RESET);
}



void gpiospi_write_int32(uint32_t data)
{
  for (int i=31; i>=0; i--)
  {
    uint32_t bit = data & (0x1<<i);
    GPIO_WriteBit(GPIOD, GPIO_Pin_15, bit ? SET : RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_15, bit ? SET : RESET);
    wait_own(wait_data);
    GPIO_WriteBit(GPIOD, GPIO_Pin_13, SET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_13, SET);
    wait_own(wait_data);
    GPIO_WriteBit(GPIOD, GPIO_Pin_13, RESET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_13, RESET);
    wait_own(wait_data);
  }
}
