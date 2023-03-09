#include "uart.h"
#include "timer.h"
#include "gait.h"
#include "math.h"
#include <stdlib.h> 
# include <string.h>
# include <stdio.h>
#include "VOFA.h"
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os 使用	  
#endif	

int onestep=0;
#if EN_UART7_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 UART_RX_BUF[UART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 UART_RX_STA=0;       //接收状态标记	
u8 res;
u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
UART_HandleTypeDef huart7; //UART句柄
u8 rdata[RDATASIZE];
char receive[receivedata];
void MY_UART7_Init(void)
{	
	//UART 初始化设置
	huart7.Instance=UART7;					    //USART1
	huart7.Init.BaudRate=115200;				    //波特率
	huart7.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	huart7.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	huart7.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	huart7.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	huart7.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&huart7);					    //HAL_UART_Init()会使能UART1
	
	HAL_UART_Receive_IT(&huart7,(u8 *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量 
}

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==UART7)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			//使能GPIOB时钟
		__HAL_RCC_UART7_CLK_ENABLE();			//使能USART7时钟
	
		GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_4;			//PB3,4
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
		GPIO_Initure.Alternate=GPIO_AF11_UART7;	//复用为USART7
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	
		
#if EN_UART7_RX
					//使能USART1中断通道
		HAL_NVIC_SetPriority(UART7_IRQn,3,3);			//抢占优先级3，子优先级3
		HAL_NVIC_EnableIRQ(UART7_IRQn);	
#endif
	}
}

void split(char *src,const char *separator,char **dest,int *num) 
{
	/*
		src ????????(buf???) 
		separator ???????
		dest ?????????
		num ??????????
	*/
     char *pNext;
     int count = 0;
     if (src == NULL || strlen(src) == 0) 
        return;
     if (separator == NULL || strlen(separator) == 0) 
        return;
     pNext = (char *)strtok(src,separator); 
     while(pNext != NULL) {
          *dest++ = pNext;
          ++count;
         pNext = (char *)strtok(NULL,separator);  
    }  
    *num = count;
}
u8 res = 0;
char *revbuf[8] = {0}; 
float x_L;
float z_H;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	u8 rec; 
	u8 sb;
	if(huart->Instance==UART7)//如果是串口1
	{				
		rec=*(huart->pRxBuffPtr-1);
		//example_uart_callback(rec);
		if(rec!=0x0D && rec!=0x0A)
		{
			HAL_UART_Transmit(&huart7,&rec,1,0xFFFF);
			receive[res]=rec;			
			res++;			
		}
		else if(rec==0x0D)
		{
		for(u8 count=0; count<receivedata-res; count++)
			{
			receive[res+count] = 'b';			
			sb =	res+count;						
			}
		}	
		if(sb==19)
		{			
			int i;
			int num = 0;
			split(receive,",",revbuf,&num);
			
			x_L=atoi(revbuf[0]);
			z_H=atoi(revbuf[1]);	
			
		//lr_output_message("%s\n",revbuf[i]);
		//HAL_UART_Transmit(&huart7,(revbuf[i]),sizeof(revbuf[i]),1000);
			res=0;				
		}											
	}	
}	


void UART7_IRQHandler(void)                	
{ 
	HAL_UART_IRQHandler(&huart7);	//调用HAL库中断处理公用函数	
	HAL_UART_Receive_IT(&huart7,(u8 *)rdata, RDATASIZE);
	//my_data_buffer[1] = 223.5;
	//example_uart_callback(*rdata);
}	
#endif

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance==UART7)//如果是串口1
//	{
//		if((UART_RX_STA&0x8000)==0)//接收未完成
//		{
//			if(UART_RX_STA&0x4000)//接收到了0x0d
//			{
//				if(aRxBuffer[0]!=0x0a)UART_RX_STA=0;//接收错误,重新开始
//				else UART_RX_STA|=0x8000;	//接收完成了 
//			}
//			else //还没收到0X0D
//			{	
//				if(aRxBuffer[0]==0x0d)UART_RX_STA|=0x4000;
//				else
//				{
//					UART_RX_BUF[UART_RX_STA&0X3FFF]=aRxBuffer[0] ;
//					UART_RX_STA++;
//					if(UART_RX_STA>(UART_REC_LEN-1))UART_RX_STA=0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//		}


//		for(u8 count=0; count<huart->RxXferSize; count++)
//		{
//			rdata[count] = *(huart->pRxBuffPtr - huart->RxXferSize + count);
//			if(rdata[count] == 0x0A)
//			break;
//		}
//		rec=*(huart->pRxBuffPtr - huart->RxXferSize);
//		rec ++;
//		if(rec >= RDATASIZE || rdata[rec-1]== 0x0a)
//		{
//			rec=0;	
//		}
//		HAL_UART_Transmit(&huart7, (u8 *)rdata,RDATASIZE,0xFFFF);
// 		HAL_UART_Receive_IT(&huart7, (u8 *)rdata, RDATASIZE);

//	}
//} 

//			return 0;

//			u8 callback=0; 
//			u8 callback1=0;			
//				while(receive[callback]!=0x2c&&receive[callback]!='b')
//				{
//					out_put_1[callback1]=receive[callback];
//					callback1++;
//					callback++;			
//				}
//				if(receive[callback]==0x2c)
//				{
//					callback++;
//					u8 callback2=0;
//					while(receive[callback]!=0x2c&&receive[callback]!='b')
//					{
//						out_put_2[callback2]=receive[callback];
//						callback2++;
//						callback++;			
//					}
//					if(receive[callback]==0x2c)
//					{
//						callback++;
//						u8 callback3=0;
//						while(receive[callback]!=0x2c&&receive[callback]!='b')
//						{
//							out_put_3[callback3]=receive[callback];
//							callback3++;
//							callback++;					
//						}					
//							
//				}		
//			}
//	