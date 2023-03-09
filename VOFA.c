/*
 * VOFA.c
 *
 *  Created on: 2020年8月14日
 *      Author: WHY
 */
#include "VOFA.h"
#include "gait.h"

float my_data_buffer[6] = {0};//待发送浮点数据缓冲区
uint8_t Send_channels;
uint8_t text_success1[32]={'1','c','h','a','n','g','e','1'};
uint8_t text_success2[32]={'1','c','h','a','n','g','e','2'};
uint8_t text_success3[32]={'1','c','h','a','n','g','e','3'};
uint8_t text_success4[32]={'1','c','h','a','n','g','e','4'};
uint8_t text_success5[32]={'1','c','h','a','n','g','e','5'};
uint8_t float_to_uint8(float num, uint8_t bit)
{
	uint8_t* temp;

	temp = (uint8_t *) &num;
	return  *(temp + bit);
}
float uint8_to_float(uint8_t* p)
{
	float temp;
	*((uint8_t*)&temp) = *p;
	*((uint8_t*)&temp+1) = *(p+1);
	*((uint8_t*)&temp+2) = *(p+2);
	*((uint8_t*)&temp+3) = *(p+3);
	return temp;
}
//串口1中断后需要执行的函数
void example_uart_callback(uint8_t userData)
{
    static uint8_t state = 0,arg_num = 0,recv_char[4];
    float recv_float;
	NRF24L01_TX_Mode();
	//volt+接受状态机
	switch(state)
	{
		case 0:
			if(userData == 0x23)//帧头第一字节
				state = 1;
			else
				state = 0;
			break;
		case 1:
			if(userData == 0x02)//帧头第二字节
				state = 2;
			else
				state = 0;
			break;
		case 2:
			arg_num = userData;//数据类别
			state = 3;
			break;
		case 3:
			recv_char[0] = userData;//数据第一字节
			state = 4;
			break;
		case 4:
			recv_char[1] = userData;//数据第二字节
			state = 5;
			break;
		case 5:
			recv_char[2] = userData;//数据第三字节
			state = 6;
			break;
		case 6:
			recv_char[3] = userData;//数据第四字节
			state = 7;
			break;
////		case 7:
////			//flag_isrun=userData;
////		if(userData=='1'||userData=='0')  flag_isrun=userData;
////		else flag_isrun=flag_isrun;
////		
////			state = 8;
////			break;
		case 7:
			if(userData == 0xaa)//帧尾
			{
				recv_float = uint8_to_float(recv_char);//四字节转化为浮点数

				switch(arg_num)
				{
					case 1:
						//my_data_buffer[1] = recv_float;
//						NRF24L01_TX_Mode();
//					NRF24L01_TxPacket(text_success1);
//					NRF24L01_RX_Mode();
						//HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
						//mc.targetCoMVelocity.element[0][0] = recv_float;
//						pid2.kp = recv_float;
//						if(flash_check(0,0))	eeprom_erase_sector(0);
//						write_buf=float_conversion_uint32(pid2.kp);
//						eeprom_page_program(0, 0, &write_buf);
//						//write_pid_to_flash();
						break;
					case 2:		//	targetCoMVelocity
						mc.targetCoMVelocity.element[ recv_char[0] ][0] = recv_float;
						NRF24L01_TX_Mode();
					NRF24L01_TxPacket(text_success2);
//					NRF24L01_RX_Mode();
						break;
					case 3:	//	gaitMode
						updateStatus = 1;
						gaitMode = recv_char[3];
						NRF24L01_TX_Mode();
					NRF24L01_TxPacket(text_success3);
//					NRF24L01_RX_Mode();
						break;
					case 4:	//	timeGait
						updateStatus = 1;
						timeGaitUpdate = recv_float;
						NRF24L01_TX_Mode();
					NRF24L01_TxPacket(text_success4);
//					NRF24L01_RX_Mode();
						break;
					case 5:	//	the pressure height in the end of swing phase
						updateStatus = 1;
						for(int i=0; i<4; i++)	//	LF RF LH RH
					pressHightBuffer[i]= (float)recv_char[i];
					NRF24L01_TX_Mode();
					NRF24L01_TxPacket(text_success5);
//					NRF24L01_RX_Mode();
						break;
						
					case 6:
						flag_isrun=recv_char[3];
//					NRF24L01_TX_Mode();
//					NRF24L01_TxPacket(text_success1);
					//NRF24L01_RX_Mode();
						break;
				}
			}
			state = 0; //状态复位，等待下一次接收
			break;
	}
}

//更新待发送浮点数据缓冲区
void store_my_data_buffer(void)
{
	my_data_buffer[0] = 123;//mc.targetCoMVelocity.element[0][0];
	 my_data_buffer[1] = 3.0;
//	my_data_buffer[2] = realSpeed30;
//	my_data_buffer[3] = realSpeed40;
//	my_data_buffer[4] = pid2.kp;
//	my_data_buffer[5] = pid2.ki;
//	my_data_buffer[6] = XY.y;
//	my_data_buffer[7] = distance00;
//	my_data_buffer[8] = distance11;

	Send_channels=4;	// max 6
}

//volt+上位机发送函数
//buffer_addr  	  发送float数据地址
//channels     	  发送float数据通道数
//my_data_buffer 待发送浮点数据缓冲区
void vofa_send_lines(void)
{
	uint32_t j,k = 0;//i = 1,
	u8 buffer_to_send[32];
	//u8 test[32]={0x01,0x02};
 // u8 buffer_to_send2[32]={0,0x02,0x02,0x70,0x40,0x00,0x00,0xf6,0x42,0x00,0x00,0x40,0x40,0x01,0x01,0x80,0x7f};
	u8 ii=1;
	buffer_to_send[ii++] = 0x02;
	buffer_to_send[ii++] = 0x02;
	buffer_to_send[ii++] = 0x70;
	//buffer_to_send[ii++] = 0x40;
	store_my_data_buffer();//更新待发送浮点数据缓冲区
	while(k < Send_channels)
	{
		for(j = 0;j < 4;j++)
		{
			buffer_to_send[ii] = float_to_uint8(* (my_data_buffer + k),j);
			ii++;
		}
		k++;
	}

    //四字节帧尾
	buffer_to_send[ii++] = 0x01;
	buffer_to_send[ii++] = 0x01;
	buffer_to_send[ii++] = 0x80;
	//buffer_to_send[ii++] = 0x7f;
	
	//NRF24L01_TX_Mode();
	//HAL_UART_Transmit(&huart7,buffer_to_send,i,0xffff);//sizeof(buffer_to_send)
	NRF24L01_TxPacket(buffer_to_send);
	//NRF24L01_TxPacket(test);
	//NRF24L01_TxPacket(buffer_to_send2);
}



