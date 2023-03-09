#include "sys.h"
#include <stdlib.h>
#include "string.h"
#include "VOFA.h"
#include "delay.h"
#include "uart.h" 
#include "pwm.h"
#include "timer.h"
#include "adc.h"
#include "can.h"
#include "gait.h"
#include "SV.h"
#include "matrix.h"
#include "24l01.h"

struct CreepMotionControl mc;
u8 TEMP1;
u8 tmp_buf1[32]={'5'},tmp_buf[32];
u8 flag_isrun='0';
u8 flag_receiveInRun=0;
u8 firstrun=1;
u8 RUN[32]={'1','r','u','n'};
u8 STOP[32]={'1','S','T','O','P'};
u8 ERROR123[32]={'1','e','r','r','o','r'};

u8 abcd[3]={0x00,0x01};
int main(void){	
	u8 c=0;	
	u8 d=255;
	Cache_Enable();                
	HAL_Init();				        	
	Stm32_Clock_Init(160,5,2,4);  	    //设置时钟,400Mhz 4分频
	delay_init(400);		
	FDCAN1_Mode_Init(5,8,31,8,FDCAN_MODE_NORMAL);  
	HAL_Delay(100);	           
	
	SV_Init();
	pid_value_init();
	TIM1_PWM_Init(10000-1,200-1);	// 100M/200=500k的计数频率, ARR自动重装载为10000, 那么PWM频率为500k/10k=50HZ
	TIM3_PWM_Init(10000-1,200-1); 
	TIM4_PWM_Init(10000-1,200-1);
	TIM12_PWM_Init(10000-1,200-1); 
	TIM15_PWM_Init(10000-1,200-1); 	
	TIM16_PWM_Init(200-1,10000-1);//pump

	NRF24L01_Init(); 	 // must after TIM4_PWM_Init	PD15     ------> TIM4_CH4 

	CLASSMC_initiation(&mc);
	float t_tCV[]={ 3, 0.0,	0.0};		
	CLASSMC_setCoMVel(&mc, t_tCV);	
	
	HAL_Delay(50);
	MX_ADC1_Init();
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);//vcc_led
	SV_ALL_N();
	//SV_ALL_CLOSE();
	HAL_Delay(1000);
	
	TIM2_Init(10000-1,1000-1);	//	10hz
	TIM5_Init(10000-1,200-1);		// 	50hz
NRF24L01_RX_Mode();	
	while(1)
	{			
			if(NRF24L01_RxPacket(tmp_buf)==0)
		{
				for(int k=1;k<=tmp_buf[0];k++)
				example_uart_callback( tmp_buf[k]);
			
			if(flag_isrun=='1')
			{
				NRF24L01_TX_Mode();
				if(firstrun==1)
				{
				NRF24L01_TxPacket(RUN);
					firstrun=0;
				}
				//vofa_send_lines();
				
			}
			else if(flag_isrun=='0')
			{
				NRF24L01_TX_Mode();
				NRF24L01_TxPacket(STOP);
				firstrun=1;
			}
			else
			{
				
				NRF24L01_TX_Mode();
				NRF24L01_TxPacket(ERROR123);
			}
			
			NRF24L01_RX_Mode();
			
		}	
		
		if(TIM5_20msFlag >= 1)
		{
			
			TIM5_20msFlag = 0;
			if(flag_isrun=='1')
		{
			updateGaitParameter(&mc);

			air_control_amble(&mc);
			// air_control_trot(&mc);
			CLASSMC_nextStep(&mc);
			CLASSMC_inverseKinematics(&mc);
			CLASSMC_setJointPosition(&mc);
		}
		}
		
		if(TIM2_100msFlag >= 2)
		{			
			TIM2_100msFlag = 0;
			
			Read_All_Ad();			
			battery_indicatior();

			


			}
		
	
		
	}
	CLASSMC_freeMatrix(&mc);
}
