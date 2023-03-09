#ifndef _CAN_H
#define _CAN_H
#include "sys.h"
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//FDCAN驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2018/6/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//FDCAN1接收RX0中断使能
#define FDCAN1_RX0_INT_ENABLE	0		//0,不使能;1,使能.
extern u8 mesg[4];
extern u8 canbuf1[8];
extern u8 canbuf2[8];
extern u8 canbuf3[4];
u8 FDCAN1_Mode_Init(u16 presc,u8 ntsjw,u16 ntsg1,u8 ntsg2,u32 mode);
u8 FDCAN1_Send_Msg(u8* msg,u32 len);
u8 FDCAN1_Receive_Msg(u8 *buf);
#endif
