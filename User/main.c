#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"
#include <stm32f10x_spi.h>
#include "stm32f10x.h"
#include "loragw_spi.h"
#include "loragw_reg.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BufferSize 33


GPIO_InitTypeDef 					GPIO_InitStructure;
SPI_InitTypeDef   				SPI_InitStructure;
NVIC_InitTypeDef 					NVIC_InitStructure;


// SPI1 : MASTER.
//SPI2	:	SLAVE

uint8_t SPI_MASTER_Buffer_Tx[BufferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                                            0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C,
                                            0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
                                            0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                                            0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E,
                                            0x1F, 0x20};
uint8_t SPI_MASTER_Buffer_Rx[BufferSize];
																						
uint8_t SPI_SLAVE_Buffer_Tx[BufferSize];
uint8_t SPI_SLAVE_Buffer_Rx[BufferSize];
uint8_t M_Tx = 0, M_Rx = 0;
uint8_t S_Tx = 0, S_Rx = 0;
uint8_t Flag_end=0;
																						
void GPIO_Configuration(void);
void Delay_ms1(uint16_t time);
void Delay_ns(uint16_t time);
void SPI1_Master_Con(void);
void SPI2_Slave_Con(void);
											
void NVIC_Configuration(void)
	{
				
				/* 1 bit for pre-emption priority, 3 bits for subpriority */
				NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

				/* Configure and enable SPI_MASTER interrupt -------------------------------*/
				NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
				NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStructure);

				/* Configure and enable SPI_SLAVE interrupt --------------------------------*/
				NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
				NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
				NVIC_Init(&NVIC_InitStructure);
		
		
	}																					
int main(void)
{
	int i;
	uint8_t buff[33];
	uint8_t data;
	SysTick_Init();
	USART_Config();
	printf("test thoi ma.");
	GPIO_Configuration();
	SX1301_SPI_Config();
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
			//SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
	//SPI1_Master_Con();
	SPI2_Slave_Con();
	NVIC_Configuration();
	Delay_ms(10);
	for (i = 0; i < 33; ++i) {
		buff[i] = 0x23;
	}
	
	lgw_connect();
	int32_t test_value = 1; /* 11000101b */\
	int32_t read_value;
	lgw_reg_w(16, test_value);
	lgw_reg_r(16, &read_value);
	printf("IMPLICIT_PAYLOAD_LENGHT = %d (should be %d)\n", read_value, test_value);
	
//	struct lgw_conf_board_s boardconf;
//	struct lgw_conf_rxrf_s rfconf;
//	struct lgw_conf_rxif_s ifconf;
//	struct lgw_tx_gain_lut_s txlut;
//	
//	memset(&boardconf, 0, sizeof boardconf);
//	boardconf.lorawan_public = true;
//	boardconf.clksrc = 1;
//	if (lgw_board_setconf(boardconf) != LGW_HAL_SUCCESS) {
//                printf("WARNING: Failed to configure board\n");
//	}
//	
//	
//	memset(&rfconf, 0, sizeof rfconf);
//	rfconf.enable = true;
//	rfconf.freq_hz = 867500000;
//	rfconf.rssi_offset = -166.0;
//	rfconf.tx_enable = true;
//	rfconf.type = LGW_RADIO_TYPE_SX1257;
//	
//	if (lgw_rxrf_setconf(1, rfconf) != LGW_HAL_SUCCESS) {
//			printf("WARNING: invalid configuration for radio %i\n", 1);
//		}
//	
//		
//	memset(&ifconf, 0, sizeof ifconf);
//	ifconf.enable = true;
//	ifconf.rf_chain = 1;
//	ifconf.freq_hz = -200000;
//	ifconf.bandwidth = BW_250KHZ;
//	ifconf.datarate = DR_LORA_SF7;
//	if (lgw_rxif_setconf(8, ifconf) != LGW_HAL_SUCCESS) {
//			printf("WARNING: invalid configuration for Lora standard channel\n");
//		}

//	memset(&ifconf, 0, sizeof ifconf);
//	ifconf.enable = true;
//	ifconf.rf_chain = 1;
//	ifconf.freq_hz = 3000000;
//	ifconf.datarate = 50000;
//	ifconf.bandwidth = BW_125KHZ;
//	
//	if (lgw_rxif_setconf(9, ifconf) != LGW_HAL_SUCCESS) {
//			printf("WARNING: invalid configuration for Lora standard channel\n");
//		}
//	printf("bat dau: ");
//	int x = lgw_start();
//	printf("x = : %d", x);
//	struct lgw_pkt_rx_s rxpkt[8];
//	struct lgw_pkt_rx_s *p;
//	int nb_pkt = lgw_receive(8, rxpkt);
//	p = &rxpkt[1];
//	printf("\nstatus= : %d", p->status);
//	int i;
//	for(i = 0; i < 255; i++)
//		printf("x = : %d", *((p->payload) + i));
//	
  while (1)
  {		
		
//			SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
			//SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
//			Delay_ms(10);
//			GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction) (0));
//			S_Rx = 0;
//			M_Tx = 0;
//			M_Rx = 0;
//			
//			lgw_spi_w(0x01, 0x10);
//			lgw_spi_rb(0x01, buff, 16);
//			printf("data read: ");
//			for(i = 0; i < 16; i++)
//				printf("%x", buff[i]);
//			for(M_Tx=0;M_Tx<33;M_Tx++)
//					{
//						while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}
//						SPI_I2S_SendData(SPI1, SPI_MASTER_Buffer_Tx[M_Tx]);
//					}
			Delay_ms(2000);

		if(Flag_end==1)
		{
			Flag_end=0;
			SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, DISABLE);
			SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, DISABLE);
			GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction) (1));
			printf("da truyen xong");
		}
  }
}

void GPIO_Configuration(void)
{
	//RCC_PCLK2Config(RCC_HCLK_Div2); 
	USART_Config();
	printf("test thoi ma");
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction) (0));
}

void SPI1_Master_Con(void)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1|RCC_APB2Periph_GPIOA, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;					
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;	
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;		
		//SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPI1, &SPI_InitStructure);		
		
		
		SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, DISABLE);
		SPI_Cmd(SPI1, ENABLE);	
	}
void SPI2_Slave_Con(void)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
		SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;		
		//SPI_InitStructure.SPI_CRCPolynomial = 7;
		SPI_Init(SPI2, &SPI_InitStructure);			
		
		SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, DISABLE);
		SPI_Cmd(SPI2, ENABLE);
	}


