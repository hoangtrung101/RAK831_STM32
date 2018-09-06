#ifndef _LORAGW_SPI_H
#define _LORAGW_SPI_H

#include <stm32f10x_spi.h>
#include "stm32f10x.h"
#include <stdio.h>
#include "bsp_usart.h"
//#include "bsp_SysTick.h"

// SPI  SX1301
#define SX1301_SPI							SPI1
#define SX1301_SPI_APBxClock_FUN			RCC_APB2PeriphClockCmd
#define SX1301_SPI_CLK						RCC_APB2Periph_SPI1

#define SX1301_RESET_PORT 					GPIOA
#define SX1301_RESET_APBxClock_FUN 			RCC_APB2PeriphClockCmd    
#define SX1301_RESET_Clock					RCC_APB2Periph_GPIOA
#define SX1301_RESET_PIN 					GPIO_Pin_8

#define SX1301_CS_PORT 						GPIOA
#define SX1301_CS_APBxClock_FUN 			RCC_APB2PeriphClockCmd    
#define SX1301_CS_Clock						RCC_APB2Periph_GPIOA
#define SX1301_CS_PIN 						GPIO_Pin_4

#define SX1301_MOSI_PORT 					GPIOA
#define SX1301_MOSI_APBxClock_FUN 			RCC_APB2PeriphClockCmd    
#define SX1301_MOSI_Clock					RCC_APB2Periph_GPIOA
#define SX1301_MOSI_PIN 					GPIO_Pin_7

#define SX1301_MISO_PORT 					GPIOA
#define SX1301_MISO_APBxClock_FUN 			RCC_APB2PeriphClockCmd    
#define SX1301_MISO_Clock					RCC_APB2Periph_GPIOA
#define SX1301_MISO_PIN 					GPIO_Pin_6

#define SX1301_SCK_PORT 					GPIOA
#define SX1301_SCK_APBxClock_FUN 			RCC_APB2PeriphClockCmd    
#define SX1301_SCK_Clock					RCC_APB2Periph_GPIOA
#define SX1301_SCK_PIN 	    				GPIO_Pin_5

#define SX1301_CS_LOW()						GPIO_ResetBits( SX1301_CS_PORT, SX1301_CS_PIN )
#define SX1301_CS_HIGH()					GPIO_SetBits( SX1301_CS_PORT, SX1301_CS_PIN )

#define SX1301_RESET_LOW()					GPIO_ResetBits( SX1301_RESET_PORT, SX1301_RESET_PIN )
#define SX1301_RESET_HIGH()					GPIO_SetBits( SX1301_RESET_PORT, SX1301_RESET_PIN )

#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))
#define Dummy_Byte                0xFF

#define LGW_SPI_SUCCESS	 0
#define LGW_SPI_ERROR	-1
#define LGW_BURST_CHUNK	 1024


void SX1301_SPI_Config();
void SX1301_Reset();
u8 SX1301SendByte(uint8_t data);
u8 SX1301ReadByte(void);

int lgw_spi_w(uint8_t address, uint8_t data);
int lgw_spi_r(uint8_t address, uint8_t *data);
int lgw_spi_wb(uint8_t address, uint8_t *data, uint16_t size);
int lgw_spi_rb(uint8_t address, uint8_t *data, uint16_t size);

static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);


#endif
