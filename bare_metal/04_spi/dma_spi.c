#include <stm32f4xx.h>

void init_spi_general(void);


// DMA SPI
extern uint16_t * const SPIBuffer;
void init_dma_spi(void);
void start_dma_spi(void);


uint16_t * const SPIBuffer = (uint16_t*)0x20000000;


// DMA SPI
void init_dma_spi(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);


  init_spi_general();

  SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
  
  SPI_Cmd(SPI2, ENABLE);


  // DMA Channel 5 - SPI TX
  DMA_Init(DMA1_Stream5, &(DMA_InitTypeDef){
      .DMA_BufferSize = 4,
      .DMA_DIR = DMA_DIR_MemoryToPeripheral,
      .DMA_Memory0BaseAddr = (uint32_t)SPIBuffer,
      .DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord,
      .DMA_MemoryInc = DMA_MemoryInc_Enable,
      .DMA_Mode = DMA_Mode_Normal,
      .DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR,
      .DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord,
      .DMA_PeripheralInc = DMA_PeripheralInc_Disable,
      .DMA_Priority = DMA_Priority_High,
    });

  DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);
   
  NVIC_Init(&(NVIC_InitTypeDef){
      .NVIC_IRQChannel = DMA1_Stream5_IRQn,
      .NVIC_IRQChannelPreemptionPriority = 0,
      .NVIC_IRQChannelSubPriority = 0,
      .NVIC_IRQChannelCmd = ENABLE,
    });
}

void start_dma_spi(void)
{
  // DMA Start
  DMA_Cmd(DMA1_Stream5, DISABLE);
  DMA_SetCurrDataCounter(DMA1_Stream5, 4);
   
  // Chip Select Low
  GPIO_WriteBit(GPIOB, GPIO_Pin_12, RESET);
   
  DMA_Cmd(DMA1_Stream5, ENABLE);
}



void spi_handleDMA1Ch5Interrupt(void);

void DMA1_Channel5_IRQHandler(void){
  spi_handleDMA1Ch5Interrupt();
  DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
}

void spi_handleDMA1Ch5Interrupt(void){
  GPIO_SetBits(GPIOD, GPIO_Pin_13);
}
