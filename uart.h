#ifndef __UART_H
#define __UART_H
#include "sys.h"
#include "stdio.h"	

#define UART_REC_LEN  			3  	//定义最大接收字节数 200
#define EN_UART7_RX 			1		//使能（1）/禁止（0）串口1接收
#define RXBUFFERSIZE   1 
#define RDATASIZE   1 
#define receivedata 20

void MY_UART7_Init(void);
extern UART_HandleTypeDef huart7;
extern char receive[receivedata];


extern int onestep;
extern char *revbuf[8];
uint16_t UART_ReceiveData(USART_TypeDef* huart7);
extern u8  UART_RX_BUF[UART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 UART_RX_STA;         		//接收状态标记	

extern u8 aRxBuffer[RXBUFFERSIZE];
extern u8 rdata[RDATASIZE];
extern float x_L;
extern float z_H;
void split(char *src,const char *separator,char **dest,int *num);
#endif 

//#define outputdata 4
//extern u8 out_put_1[outputdata];
//extern u8 out_put_2[outputdata];
//extern u8 out_put_3[outputdata];
//extern u8 callback1;
//extern u8 callback2;
//extern u8 callback3;