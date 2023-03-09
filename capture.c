#include "capture.h"

uint16_t 	Time8Channel1HighTime = 0, Time8Channel2HighTime = 0, Time8Channel3HighTime = 0,Time8Channel4HighTime = 0;
uint8_t  	Time8Channel1Edge = 0, Time8Channel2Edge = 0, Time8Channel3Edge = 0, Time8Channel4Edge = 0;

uint16_t  Time8Channel1RisingTimeNow = 0, Time8Channel1FallingTime = 0;
uint16_t 	Time8Channel2RisingTimeNow = 0, Time8Channel2FallingTime = 0;
uint16_t 	Time8Channel3RisingTimeNow = 0, Time8Channel3FallingTime = 0;
uint16_t 	Time8Channel4RisingTimeNow = 0, Time8Channel4FallingTime = 0;

u8 pause_time=0;
u8 operating_flag=0;

TIM_HandleTypeDef TIM8_Handler; 


void TIM8_Cap_Init(void) 
{  

    TIM_IC_InitTypeDef TIM8_Config;  
    

    TIM8_Handler.Instance=TIM8;                          //通用定时器5
    TIM8_Handler.Init.Prescaler=100-1;                     //分频    100m/100=1m
    TIM8_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM8_Handler.Init.Period=5000-1;                        //自动装载值1m/5k=200
    TIM8_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;	
	  TIM8_Handler.Init.RepetitionCounter = 0;
    TIM8_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	
    HAL_TIM_IC_Init(&TIM8_Handler);
    
    TIM8_Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
    TIM8_Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
    TIM8_Config.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM8_Config.ICFilter=0;                          //配置输入滤波器，不滤波
	
    HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_Config,TIM_CHANNEL_1);//配置TIM8通道1
		HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_Config,TIM_CHANNEL_2);//配置TIM8通道2
		HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_Config,TIM_CHANNEL_3);//配置TIM8通道3
		HAL_TIM_IC_ConfigChannel(&TIM8_Handler,&TIM8_Config,TIM_CHANNEL_4);//配置TIM8通道4
	
    HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_1);   //开始捕获TIM8的通道1
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_2);   //开始捕获TIM8的通道1
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_3);   //开始捕获TIM8的通道1
		HAL_TIM_IC_Start_IT(&TIM8_Handler,TIM_CHANNEL_4);   //开始捕获TIM8的通道1
		
    __HAL_TIM_ENABLE_IT(&TIM8_Handler,TIM_IT_UPDATE);   //使能更新中断
}


void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM8_CLK_ENABLE();            //使能TIM8时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
	
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;           //PA0
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;      //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;        //shangla  未捕获为低电平
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_LOW;     //高速
    GPIO_Initure.Alternate=GPIO_AF3_TIM8;   //
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);

    HAL_NVIC_SetPriority(TIM8_CC_IRQn,1,1);    //设置中断优先级，抢占优先级1，子优先1
    HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);          //开启ITM8中断   
}



void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM8)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			if(Time8Channel1Edge == 0)
			{
				Time8Channel1RisingTimeNow = HAL_TIM_ReadCapturedValue(&TIM8_Handler, TIM_CHANNEL_1);
				__HAL_TIM_SET_CAPTUREPOLARITY(&TIM8_Handler, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
				HAL_TIM_IC_Start_IT(&TIM8_Handler, TIM_CHANNEL_1);
				Time8Channel1Edge = 1;
			}
			else if(Time8Channel1Edge == 1)
			{
				Time8Channel1FallingTime = HAL_TIM_ReadCapturedValue(&TIM8_Handler, TIM_CHANNEL_1);	
				__HAL_TIM_SET_CAPTUREPOLARITY(&TIM8_Handler, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
				HAL_TIM_IC_Start_IT(&TIM8_Handler, TIM_CHANNEL_1);	
				if(Time8Channel1FallingTime < Time8Channel1RisingTimeNow)
				{
					Time8Channel1HighTime = Time8Channel1FallingTime + 5000 - Time8Channel1RisingTimeNow;
				}
				else if(Time8Channel1FallingTime > Time8Channel1RisingTimeNow)
				{
					Time8Channel1HighTime = Time8Channel1FallingTime - Time8Channel1RisingTimeNow;
				}
			Time8Channel1Edge = 0;
			}
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			if(Time8Channel2Edge == 0)
			{
				Time8Channel2RisingTimeNow = HAL_TIM_ReadCapturedValue(&TIM8_Handler, TIM_CHANNEL_2);
				__HAL_TIM_SET_CAPTUREPOLARITY(&TIM8_Handler, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
				HAL_TIM_IC_Start_IT(&TIM8_Handler, TIM_CHANNEL_2);
				Time8Channel2Edge = 1;
			}
			else if(Time8Channel2Edge == 1)
			{
				Time8Channel2FallingTime = HAL_TIM_ReadCapturedValue(&TIM8_Handler, TIM_CHANNEL_2);	
				__HAL_TIM_SET_CAPTUREPOLARITY(&TIM8_Handler, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
				HAL_TIM_IC_Start_IT(&TIM8_Handler, TIM_CHANNEL_2);
			
				if(Time8Channel2FallingTime < Time8Channel2RisingTimeNow)
				{
					Time8Channel2HighTime = Time8Channel2FallingTime + 5000 - Time8Channel2RisingTimeNow;
				}
				else if(Time8Channel2FallingTime > Time8Channel2RisingTimeNow)
				{
					Time8Channel2HighTime = Time8Channel2FallingTime - Time8Channel2RisingTimeNow;
				}
			Time8Channel2Edge = 0;
			}
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			if(Time8Channel3Edge == 0)
			{
				Time8Channel3RisingTimeNow = HAL_TIM_ReadCapturedValue(&TIM8_Handler, TIM_CHANNEL_3);
				__HAL_TIM_SET_CAPTUREPOLARITY(&TIM8_Handler, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
				HAL_TIM_IC_Start_IT(&TIM8_Handler, TIM_CHANNEL_3);
				Time8Channel3Edge = 1;				
			}
			else if(Time8Channel3Edge == 1)
			{
				Time8Channel3FallingTime = HAL_TIM_ReadCapturedValue(&TIM8_Handler, TIM_CHANNEL_3);	
				__HAL_TIM_SET_CAPTUREPOLARITY(&TIM8_Handler, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
				HAL_TIM_IC_Start_IT(&TIM8_Handler, TIM_CHANNEL_3);
			
				if(Time8Channel3FallingTime < Time8Channel3RisingTimeNow)
				{
					Time8Channel3HighTime = Time8Channel3FallingTime + 5000 - Time8Channel3RisingTimeNow;
				}
				else if(Time8Channel3FallingTime > Time8Channel3RisingTimeNow)
				{
					Time8Channel3HighTime = Time8Channel3FallingTime - Time8Channel3RisingTimeNow;
				}
			Time8Channel3Edge = 0;
			}
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		{
			if(Time8Channel4Edge == 0)
			{
				Time8Channel4RisingTimeNow = HAL_TIM_ReadCapturedValue(&TIM8_Handler, TIM_CHANNEL_4);
				__HAL_TIM_SET_CAPTUREPOLARITY(&TIM8_Handler, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);
				HAL_TIM_IC_Start_IT(&TIM8_Handler, TIM_CHANNEL_4);
				Time8Channel4Edge = 1;
			}
			else if(Time8Channel4Edge == 1)
			{
				Time8Channel4FallingTime = HAL_TIM_ReadCapturedValue(&TIM8_Handler, TIM_CHANNEL_4);	
				__HAL_TIM_SET_CAPTUREPOLARITY(&TIM8_Handler, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
				HAL_TIM_IC_Start_IT(&TIM8_Handler, TIM_CHANNEL_4);
			
				if(Time8Channel4FallingTime < Time8Channel4RisingTimeNow)
				{
					Time8Channel4HighTime = Time8Channel4FallingTime + 5000 - Time8Channel4RisingTimeNow;
				}
				else if(Time8Channel4FallingTime >= Time8Channel4RisingTimeNow)
				{
					Time8Channel4HighTime = Time8Channel4FallingTime - Time8Channel4RisingTimeNow;
				}
			Time8Channel4Edge = 0;
			}
		}
	}
if ((Time8Channel2HighTime >=1600)&&(Time8Channel1HighTime <=1600)&&(Time8Channel1HighTime >=1400))
{
	operating_flag = 1; // run forward
	pause_time=(2000-Time8Channel2HighTime)/20+10;
}

else if ((Time8Channel2HighTime <=1400 )&&(Time8Channel1HighTime <=1600)&&(Time8Channel1HighTime >=1400))
{
	operating_flag = 2; // run back
	pause_time=30-(1400-Time8Channel2HighTime)/20;
}

else if ((Time8Channel1HighTime <=1400)&&(Time8Channel2HighTime <=1600)&&(Time8Channel2HighTime >=1400))
{
	operating_flag = 3; // left
	pause_time=30-(1400-Time8Channel1HighTime)/20;
}

else if ((Time8Channel1HighTime >=1600 )&&(Time8Channel1HighTime <=1600)&&(Time8Channel1HighTime >=1400))
{
	operating_flag = 4; // right
	pause_time=(2000-Time8Channel1HighTime)/20+10;
}

else
{
	operating_flag = 0; // pause
}
}