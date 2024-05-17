/**
  ******************************************************************************
  * @file    WIZnet MDK5 Project Template  ../main.c 
  * @author  WIZnet Software Team
  * @version V1.0.0
  * @date    2018-09-25
  * @brief   Main program body
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
  * <h2><center>&copy; COPYRIGHT 2018 WIZnet H.K. Ltd.</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/** @addtogroup USART
  * @brief 串口模块
  * @{
  */

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/**
  * @brief  开启GPIOC,串口3时钟 
  * @param  None
  * @retval None
  * @note  对于某些GPIO上的默认复用功能可以不开启服用时钟，如果用到复用功能的重
           映射，则需要开启复用时钟
  */
void USART_RCC_Configuration(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
}

/**
  * @brief  设置串口1发送与接收引脚的模式 
  * @param  None
  * @retval None
  */
void USART_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief  配置串口1，并使能串口1
  * @param  None
  * @retval None
  */
void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStruct;

	USART_RCC_Configuration();
	USART_GPIO_Configuration();

	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStruct);
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);//禁止接收中断
	USART_Cmd(USART1, ENABLE);//使能串口3
	USART_ClearFlag(USART1, USART_FLAG_TC);
}


//需要在target选项中选择使用USE microLIB

PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1,(uint8_t)ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

	return ch;
}
/**
  * @}
  */

/*********************************END OF FILE**********************************/

