/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_usart.h"
//#include "bsp_SysTick.h"
#include <stm32f10x_spi.h>
#include "loragw_spi.h"
#define BufferSize 32

extern void TimingDelay_Decrement(void);

extern uint8_t SPI_MASTER_Buffer_Tx[BufferSize],SPI_MASTER_Buffer_Rx[BufferSize],SPI_SLAVE_Buffer_Tx[BufferSize],SPI_SLAVE_Buffer_Rx[BufferSize];
extern uint8_t M_Tx, M_Rx;
extern uint8_t S_Tx, S_Rx;
extern uint8_t Flag_end;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();	
}

void SPI2_IRQHandler(void)
{
	int i;
	if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) != RESET)
		{	
			uint8_t datasend[33] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                                            0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C,
                                            0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
                                            0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
                                            0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E,
                                            0x1F, 0x20};
		
			int x  = SPI_I2S_ReceiveData(SPI2);
			printf("\nData: %x", x);
			while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}
			for(i = 0; i < 33; i++)
				SPI_I2S_SendData(SPI2, datasend[i]);
			
//			SPI_SLAVE_Buffer_Rx[S_Rx]=SPI_I2S_ReceiveData(SPI2);
//			printf("\nSPI2 Nhan: %x -", SPI_SLAVE_Buffer_Tx[S_Rx]);
//			
//			if(SPI_SLAVE_Buffer_Rx[0]==0x01)
//			{
//				
//				SPI_SLAVE_Buffer_Tx[S_Rx]=SPI_SLAVE_Buffer_Rx[S_Rx];
//				
//				while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}
//				SPI_I2S_SendData(SPI2, SPI_SLAVE_Buffer_Tx[S_Rx]);
//				S_Rx++;
//				
//			}
//			else {S_Rx=0;Flag_end=0;}
		}
	SPI_I2S_ClearFlag(SPI2, SPI_I2S_IT_RXNE);

}

void SPI1_IRQHandler(void)
{
	
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) != RESET)
		{
			int x = SPI_I2S_ReceiveData(SPI1);
			printf("\nSPI1 Nhan: %d -", x);
			if(SPI_MASTER_Buffer_Rx[0]==0x01)
			{
				
				M_Rx++;
				if(SPI_MASTER_Buffer_Rx[31]==0x20) Flag_end=1;
				
			}
			else {M_Rx=0;Flag_end=0;}
		}
		SPI_I2S_ClearFlag(SPI1, SPI_I2S_IT_RXNE);

}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
