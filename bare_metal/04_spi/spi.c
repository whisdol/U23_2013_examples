#include <stm32f4xx.h>

void wait_own(int v);


// SPI
void init_spi_general(void);
void init_spi(void); //TODO
void spi_write_int32(uint32_t data);
void wait_spi_done(void);


#define BUFFER_SIZE 2
uint8_t spiTxCounter;
volatile uint8_t spiBusyFlag;
uint16_t spiDataBuffer[BUFFER_SIZE];

 

// SPI
void init_spi_general(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  // SPI  (SCK PB13, MOSI PB15)
  GPIO_Init(GPIOB, &(GPIO_InitTypeDef){
      .GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15,
      .GPIO_Mode = GPIO_Mode_AF,
      .GPIO_Speed = GPIO_Speed_50MHz,
    });

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
   
  // Latch / (Chip Select)
  GPIO_Init(GPIOB, &(GPIO_InitTypeDef){
      .GPIO_Pin = GPIO_Pin_12,
      .GPIO_Mode = GPIO_Mode_OUT,
      .GPIO_Speed = GPIO_Speed_50MHz,
    });
   
  GPIO_WriteBit(GPIOB, GPIO_Pin_12, RESET);
   
  SPI_Init(SPI2, &(SPI_InitTypeDef){
      .SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8,
      .SPI_CPHA = SPI_CPHA_1Edge,
      .SPI_CPOL = SPI_CPOL_Low,
      .SPI_CRCPolynomial = 1,
      .SPI_DataSize = SPI_DataSize_16b,
      .SPI_Direction = SPI_Direction_1Line_Tx,
      .SPI_FirstBit = SPI_FirstBit_MSB,
      .SPI_Mode = SPI_Mode_Master,
      .SPI_NSS = SPI_NSS_Soft,
    });
}



void init_spi(void)
{
  NVIC_Init(&(NVIC_InitTypeDef){
      .NVIC_IRQChannel = SPI2_IRQn,
      .NVIC_IRQChannelPreemptionPriority = 0,
      .NVIC_IRQChannelSubPriority = 0,
      .NVIC_IRQChannelCmd = ENABLE,
    });

  init_spi_general();

  //SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
  SPI_Cmd(SPI2, ENABLE);

  spiBusyFlag = 0;
}



void spi_handleSPI2Interrupt(void);

void SPI2_IRQHandler(void){
  spi_handleSPI2Interrupt();
}

void spi_enableTxInterrupt(void){
  SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, ENABLE);
}
 
void spi_disableTxInterrupt(void){
  SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_TXE, DISABLE);
}

void wait_spi_done(void){
  while(spiBusyFlag);
  //wait_own(1);
}
 
void spi_write_int32(uint32_t data)
{
  wait_spi_done();
  spiBusyFlag = 1;
  spiTxCounter = 2;
  spiDataBuffer[0] = (data>>0) & 0xFFFF;
  spiDataBuffer[1] = (data>>16) & 0xFFFF;
  spi_enableTxInterrupt();
}
 
void spi_handleSPI2Interrupt(void){
  if(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == SET){
    //if(spiTxCounter == 0){
    //  spiBusyFlag = 0;
    //}
  }else if(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == SET){
    // Transmit Buffer Empty
    if(spiTxCounter != 0){
      SPI_I2S_SendData(SPI2, spiDataBuffer[--spiTxCounter]);
    }else{
      spi_disableTxInterrupt();
      spiBusyFlag = 0;
    }
  }
}
