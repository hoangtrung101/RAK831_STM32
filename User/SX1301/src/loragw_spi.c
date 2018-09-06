#include "loragw_spi.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include <string.h>
#include <stdint.h>		
#include <stdio.h>		
#include <stdlib.h>		

#define SPIDELAY  1


#define ARRAY_SIZE(a)  (sizeof(a) / sizeof(a[0]))

#if DEBUG_SPI == 1
	#define DEBUG_MSG(str) 			printf(str);
#endif


#define READ_ACCESS		0x00
#define WRITE_ACCESS	0x80


static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);


void SX1301_SPI_Config()
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	SX1301_Reset();
	SX1301_SPI_APBxClock_FUN(SX1301_SPI_CLK, ENABLE);
	SX1301_CS_APBxClock_FUN(SX1301_CS_Clock|SX1301_MOSI_Clock|SX1301_MISO_Clock, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SX1301_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SX1301_CS_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SX1301_RESET_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SX1301_RESET_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SX1301_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SX1301_SCK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SX1301_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SX1301_MISO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SX1301_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SX1301_MOSI_PORT, &GPIO_InitStructure);
	
	SX1301_CS_HIGH();
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SX1301_SPI, &SPI_InitStructure);
	SPI_Cmd(SX1301_SPI, ENABLE);
}

void SX1301_Reset()
{
	SX1301_RESET_HIGH();
	
	Delay_us(10);
	SX1301_RESET_LOW();
	Delay_us(10);
}


u8 SX1301SendByte(uint8_t data)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(SX1301_SPI, SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	}
	SPI_I2S_SendData(SX1301_SPI, data);
	
	SPITimeout = SPIT_FLAG_TIMEOUT;
//	while (SPI_I2S_GetFlagStatus(SX1301_SPI , SPI_I2S_FLAG_RXNE) == RESET)
//	{
//		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
//	}
	return SPI_I2S_ReceiveData(SX1301_SPI);
}

u8 SX1301ReadByte(void)
{
	return (SX1301SendByte(Dummy_Byte));
}

int lgw_spi_w(uint8_t address, uint8_t data)
{
	SX1301_CS_LOW();
	Delay_us(SPIDELAY);
	SX1301SendByte(WRITE_ACCESS | (address & 0x7F));
	SX1301SendByte(data);
	Delay_us(SPIDELAY);
	SX1301_CS_HIGH();
	return LGW_SPI_SUCCESS;
}

int lgw_spi_r(uint8_t address, uint8_t *data)
{	
	SX1301_CS_LOW();
	Delay_us(SPIDELAY);
	SX1301SendByte(READ_ACCESS | (address & 0x7F));
	*data = SX1301SendByte(0);
	Delay_us(SPIDELAY);
	SX1301_CS_HIGH();
	return LGW_SPI_SUCCESS;
	
}

int lgw_spi_wb(uint8_t address, uint8_t *data, uint16_t size)
{
	int i = 0;
	SX1301_CS_LOW();
	Delay_us(SPIDELAY);
	SX1301SendByte(WRITE_ACCESS | (address & 0x7F));
	for(i = 0; i < size; i++)
	{
		SX1301SendByte(data[i]);
	}
	Delay_us(SPIDELAY);
	SX1301_CS_HIGH();
	return LGW_SPI_SUCCESS;
}

int lgw_spi_rb(uint8_t address, uint8_t *data, uint16_t size)
{
	int i;
	SX1301_CS_LOW();
	Delay_us(SPIDELAY);
	SX1301SendByte(READ_ACCESS | (address & 0x7F));
	for(i = 0; i < size; i++)
	{
		data[i] = SX1301SendByte(0);
	}
	Delay_us(SPIDELAY);
	SX1301_CS_HIGH();
	return LGW_SPI_SUCCESS;
}

static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  printf("SPI errorCode = %d",errorCode);
  return 0;
}