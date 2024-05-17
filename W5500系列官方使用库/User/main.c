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
#include "delay.h"
#include "spi.h"
#include "socket.h"	// Just include one header for WIZCHIP
#include "string.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SOCK_TCPS        0
#define DATA_BUF_SIZE   2048
/* Private macro -------------------------------------------------------------*/
uint8_t gDATABUF[DATA_BUF_SIZE];
// Default Network Configuration
wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc,0x11, 0x11, 0x11},
                            .ip = {192, 168, 1, 123},
                            .sn = {255,255,255,0},
                            .gw = {192, 168, 1, 1},
                            .dns = {8,8,8,8},
                            .dhcp = NETINFO_STATIC };
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void platform_init(void);								// initialize the dependent host peripheral
void network_init(void);								// Initialize Network information and display it
/**
  * @brief  串口打印输出
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8_t tmp;
	int16_t ret = 0;
	uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};
	uint8_t DstIP[4]={192,168,1,101};
	uint16_t	DstPort=6000;
	//Host dependent peripheral initialized
	platform_init();
	// First of all, Should register SPI callback functions implemented by user for accessing WIZCHIP 
	/* Critical section callback */
	reg_wizchip_cris_cbfunc(SPI_CrisEnter, SPI_CrisExit);	//注册临界区函数
	/* Chip selection call back */
#if   _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_
	reg_wizchip_cs_cbfunc(SPI_CS_Select, SPI_CS_Deselect);//注册SPI片选信号函数
#elif _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_
	reg_wizchip_cs_cbfunc(SPI_CS_Select, SPI_CS_Deselect);  // CS must be tried with LOW.
#else
   #if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SIP_) != _WIZCHIP_IO_MODE_SIP_
      #error "Unknown _WIZCHIP_IO_MODE_"
   #else
      reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
   #endif
#endif
	/* SPI Read & Write callback function */
	reg_wizchip_spi_cbfunc(SPI_ReadByte, SPI_WriteByte);	//注册读写函数

	/* WIZCHIP SOCKET Buffer initialize */
	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1){
		 printf("WIZCHIP Initialized fail.\r\n");
		 while(1);
	}

	/* PHY link status check */
	do{
		 if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1){
				printf("Unknown PHY Link stauts.\r\n");
		 }
	}while(tmp == PHY_LINK_OFF);

	/* Network initialization */
	network_init();
	
	while(1)																												// Socket状态机
	{
		switch(getSn_SR(0))																						// 获取socket0的状态
		{
			case SOCK_UDP:																							// Socket处于初始化完成(打开)状态
					delay_ms(100);
					if(getSn_IR(0) & Sn_IR_RECV)
					{
						setSn_IR(0, Sn_IR_RECV);															// Sn_IR的RECV位置1
					}
					// 数据回环测试程序：数据从远程上位机发给W5500，W5500接收到数据后再回给远程上位机
					if((ret=getSn_RX_RSR(0))>0)
					{ 
						memset(gDATABUF,0,ret+1);
						recvfrom(0,gDATABUF, ret, DstIP,&DstPort);			// W5500接收来自远程上位机的数据，并通过SPI发送给MCU
						printf("%s\r\n",gDATABUF);															// 串口打印接收到的数据
						sendto(0,gDATABUF,ret, DstIP, DstPort);		  		// 接收到数据后再回给远程上位机，完成数据回环
					}
			break;
			case SOCK_CLOSED:																						// Socket处于关闭状态
					socket(0,Sn_MR_UDP,5000,0x00);												// 打开Socket0，并配置为UDP模式，打开一个本地端口
			break;
		}
	}
}
/**
  * @brief  Intialize the network information to be used in WIZCHIP
  * @retval None
  */
void network_init(void)
{
  uint8_t tmpstr[6];
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);

	// Display Network Information
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
	printf("\r\n=== %s NET CONF ===\r\n",(char*)tmpstr);
	printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],
		  gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
	printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
	printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
	printf("======================\r\n");
}

/**
  * @brief  Loopback Test Example Code using ioLibrary_BSD	
  * @retval None
  */
void platform_init(void)
{
//	SystemInit();//系统时钟初始化
	USART_Configuration();//串口1初始化	
	//Config SPI
	SPI_Configuration();
	//延时初始化
	delay_init(168);
}

/*********************************END OF FILE**********************************/

