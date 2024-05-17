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
#include "main.h"
#include "usart.h"
/**
  * @brief  配置指定SPI的引脚
  * @retval None
  */
static void SPI_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	#if 0
	//打开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//打开端口复用
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	//PB13->SCK	 					 
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;  	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//PB14->MISO
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//PB15->MOSI	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//PB12->CS,初始化片选输出引脚
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//拉高片选
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	
	#else 
	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB,ENABLE);
	
	//打开端口复用
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	//PB3->SCK	 					 
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;  	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//PB4->MISO
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_4;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//PB5->MOSI	
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//PA4->CS,初始化片选输出引脚
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//拉高片选
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	#endif
}
/**
  * @brief  根据外部SPI设备配置SPI相关参数
  * @retval None
  */
void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStruct;

	SPI_GPIO_Configuration();
	#if 0	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2,&SPI_InitStruct);
	
	SPI_Cmd(SPI2, ENABLE);
	#else
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1,&SPI_InitStruct);
	
	SPI_Cmd(SPI1, ENABLE);
	#endif
}
/**
  * @brief  写1字节数据到SPI总线
  * @param  TxData 写到总线的数据
  * @retval None
  */
void SPI_WriteByte(uint8_t TxData)
{	
	#if 0
	while((SPI2->SR&SPI_I2S_FLAG_TXE)==(uint16_t)RESET);	//等待发送区空		  
	SPI2->DR=TxData;	 	  									              //发送一个byte 
	while((SPI2->SR&SPI_I2S_FLAG_RXNE)==(uint16_t)RESET); //等待接收完一个byte  
	SPI2->DR;	
	#else
	while((SPI1->SR&SPI_I2S_FLAG_TXE)==(uint16_t)RESET);	//等待发送区空		  
	SPI1->DR=TxData;	 	  									//发送一个byte 
	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==(uint16_t)RESET); //等待接收完一个byte  
	SPI1->DR;	
	#endif	
}
/**
  * @brief  从SPI总线读取1字节数据
  * @retval 读到的数据
  */
uint8_t SPI_ReadByte(void)
{	
	#if 0
	while((SPI2->SR&SPI_I2S_FLAG_TXE)==(uint16_t)RESET);	//等待发送区空			  
	SPI2->DR=0xFF;	 	  										//发送一个空数据产生输入数据的时钟 
	while((SPI2->SR&SPI_I2S_FLAG_RXNE)==(uint16_t)RESET); //等待接收完一个byte  
	return SPI2->DR;	
  #else 
	while((SPI1->SR&SPI_I2S_FLAG_TXE)==(uint16_t)RESET);	//等待发送区空			  
	SPI1->DR=0xFF;	 	  										//发送一个空数据产生输入数据的时钟 
	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==(uint16_t)RESET); //等待接收完一个byte  	
	return SPI1->DR;	
  #endif	
}
/**
  * @brief  进入临界区
  * @retval None
  */
void SPI_CrisEnter(void)
{
	__set_PRIMASK(1);
}
/**
  * @brief  退出临界区
  * @retval None
  */
void SPI_CrisExit(void)
{
	__set_PRIMASK(0);
}

/**
  * @brief  片选信号输出低电平
  * @retval None
  */
void SPI_CS_Select(void)
{
	#if 0
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	#else 
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	#endif
}
/**
  * @brief  片选信号输出高电平
  * @retval None
  */
void SPI_CS_Deselect(void)
{
	#if 0
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	#else 
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	#endif
}
/*********************************END OF FILE**********************************/

