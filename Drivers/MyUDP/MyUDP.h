#ifndef MYUDP_H
#define MYUDP_H
#include "MyUDP.h"
#include "main.h"
#include "w5500.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "spi.h"
#include <string.h>  // memcmp
#define SOCK_UDPS  2    //定义socket号
extern  uint8_t buff[128];   //定义缓冲区                                                 
extern uint8_t remote_ip[4]; //远程IP地址
extern uint16_t remote_port; //远程端口号

void UDPinit(void);
void do_udp(void);
#endif // MYUDP_H
