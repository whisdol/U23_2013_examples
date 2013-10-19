#include <stdio.h>
#include <stdlib.h>

#include <stm32f4xx.h>


// HFSR: p/x *(uint32_t*)0xe000ed2c
#define wait_led 10000000

// General
uint32_t convert_color(uint32_t r, uint32_t g, uint32_t b);
void wait_own(int v);
void init_leds(void);
void latch(void);

// GPIO SPI
void init_gpiospi(void);
void gpiospi_write_int32(uint32_t data);

// SPI
void init_spi_general(void);
void init_spi(void); //TODO
void spi_write_int32(uint32_t data);
void wait_spi_done(void);

// DMA SPI
extern uint16_t * const SPIBuffer;
void init_dma_spi(void);
void start_dma_spi(void);


void main_gpiospi(void);
void main_spi(void);
void main_dmaspi(void);



int main()
{
  init_leds();

  //main_gpiospi();
  //main_spi();
  main_dmaspi();

  GPIO_SetBits(GPIOD, GPIO_Pin_14); 

  while(1);
}


void main_dmaspi(void)
{
  init_dma_spi();

  SPIBuffer[0] = convert_color(1023,0,0)>>16 & 0xFFFF;
  SPIBuffer[1] = convert_color(1023,0,0)>>0 & 0xFFFF;
  SPIBuffer[2] = convert_color(1023,1023,1023)>>16 & 0xFFFF;
  SPIBuffer[3] = convert_color(1023,1023,1023)>>0 & 0xFFFF;

  start_dma_spi();
  wait_own(100000000);
  latch();
}


void main_spi(void)
{
  init_spi();

  spi_write_int32(convert_color(1023,1023,1023));
  wait_spi_done();
  latch();
  wait_own(wait_led);
  spi_write_int32(convert_color(1023,0,0));
  wait_spi_done();
  latch();
  wait_own(wait_led);
  spi_write_int32(convert_color(0,1023,0));
  wait_spi_done();
  latch();
  wait_own(wait_led);
  spi_write_int32(convert_color(0,0,1023));
  wait_spi_done();
  latch();
  wait_own(wait_led);
  spi_write_int32(convert_color(0,0,0));
  wait_spi_done();
  latch();
}


void main_gpiospi(void)
{
  init_gpiospi();

  gpiospi_write_int32(convert_color(1023,1023,1023));
  latch();
  wait_own(wait_led);
  gpiospi_write_int32(convert_color(1023,0,0));
  latch();
  wait_own(wait_led);
  gpiospi_write_int32(convert_color(0,1023,0));
  latch();
  wait_own(wait_led);
  gpiospi_write_int32(convert_color(0,0,1023));
  latch();
  wait_own(wait_led);
  gpiospi_write_int32(convert_color(0,0,0));
  latch();
}


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
	ERROR_MSG("Assertion failed in file %s:%d\r\n", file, line);
	while(1);	
}
#endif
