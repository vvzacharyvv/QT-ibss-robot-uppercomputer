#include "pwm.h"

TIM_HandleTypeDef TIM1_Handler;  
TIM_HandleTypeDef TIM3_Handler;  //定时器
TIM_HandleTypeDef TIM4_Handler;  
TIM_HandleTypeDef TIM12_Handler;  
TIM_HandleTypeDef TIM15_Handler;
TIM_HandleTypeDef TIM16_Handler;

TIM_OC_InitTypeDef TIM1_CH1Handler;
TIM_OC_InitTypeDef TIM1_CH2Handler;
TIM_OC_InitTypeDef TIM1_CH3Handler;
TIM_OC_InitTypeDef TIM1_CH4Handler; 

TIM_OC_InitTypeDef TIM3_CH1Handler;
TIM_OC_InitTypeDef TIM3_CH2Handler;
TIM_OC_InitTypeDef TIM3_CH3Handler;
TIM_OC_InitTypeDef TIM3_CH4Handler; 

TIM_OC_InitTypeDef TIM4_CH1Handler;
TIM_OC_InitTypeDef TIM4_CH2Handler;
TIM_OC_InitTypeDef TIM4_CH3Handler;
TIM_OC_InitTypeDef TIM4_CH4Handler;

TIM_OC_InitTypeDef TIM12_CH1Handler;
TIM_OC_InitTypeDef TIM12_CH2Handler; 
TIM_OC_InitTypeDef TIM12_CH3Handler;
TIM_OC_InitTypeDef TIM12_CH4Handler;
TIM_OC_InitTypeDef TIM15_CH1Handler;
TIM_OC_InitTypeDef TIM15_CH2Handler; 

TIM_OC_InitTypeDef TIM16_CH1Handler;

void TIM1_PWM_Init(u16 arr,u16 psc)
{ 
    TIM1_Handler.Instance=TIM1;            //定时器3
    TIM1_Handler.Init.Prescaler=psc;       //定时器分频
    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM1_Handler.Init.Period=arr;          //自动重装载值
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_PWM_Init(&TIM1_Handler);       //初始化PWM
    
	//对应 User_PWM_SetPulse
    TIM1_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM1_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM1_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	
		TIM1_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM1_CH2Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM1_CH2Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	
		TIM1_CH3Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM1_CH3Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM1_CH3Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	
		TIM1_CH4Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM1_CH4Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM1_CH4Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	
    HAL_TIM_PWM_ConfigChannel(&TIM1_Handler,&TIM1_CH1Handler,TIM_CHANNEL_1);//配置TIM1通道1
    HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_1);//开启PWM通道1
		HAL_TIM_PWM_ConfigChannel(&TIM1_Handler,&TIM1_CH2Handler,TIM_CHANNEL_2);//配置TIM1通道2
    HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_2);//开启PWM通道2
		HAL_TIM_PWM_ConfigChannel(&TIM1_Handler,&TIM1_CH3Handler,TIM_CHANNEL_3);//配置TIM1通道3
    HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_3);//开启PWM通道3
		HAL_TIM_PWM_ConfigChannel(&TIM1_Handler,&TIM1_CH4Handler,TIM_CHANNEL_4);//配置TIM1通道4
		HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_4);//开启PWM通道4
}


void TIM3_PWM_Init(u16 arr,u16 psc)
{ 
    TIM3_Handler.Instance=TIM3;            //定时器3
    TIM3_Handler.Init.Prescaler=psc;       //定时器分频
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM3_Handler.Init.Period=arr;          //自动重装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	

    HAL_TIM_PWM_Init(&TIM3_Handler);       //初始化PWM
    
	//对应 User_PWM_SetPulse
		TIM3_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM3_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM3_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	
		TIM3_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM3_CH2Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM3_CH2Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	
    TIM3_CH3Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM3_CH3Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM3_CH3Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	
		TIM3_CH4Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM3_CH4Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM3_CH4Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	//通道配置
    HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH1Handler,TIM_CHANNEL_1);//配置TIM3通道4
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_1);//开启PWM通道4
	
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH2Handler,TIM_CHANNEL_2);//配置TIM3通道3
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_2);//开启PWM通道4
	
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH3Handler,TIM_CHANNEL_3);//配置TIM3通道4
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_3);//开启PWM通道4
	
		HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4);//配置TIM3通道3
    HAL_TIM_PWM_Start(&TIM3_Handler,TIM_CHANNEL_4);//开启PWM通道4
	
}
void TIM4_PWM_Init(u16 arr,u16 psc)
{ 
    TIM4_Handler.Instance=TIM4;            //定时器3
    TIM4_Handler.Init.Prescaler=psc;       //定时器分频
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM4_Handler.Init.Period=arr;          //自动重装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	

    HAL_TIM_PWM_Init(&TIM4_Handler);       //初始化PWM
    
	//对应 User_PWM_SetPulse
    TIM4_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM4_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM4_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 

		TIM4_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM4_CH2Handler.Pulse=arr/2;            //
    TIM4_CH2Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //
	
		TIM4_CH3Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM4_CH3Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM4_CH3Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 

		TIM4_CH4Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM4_CH4Handler.Pulse=arr/2;            //
    TIM4_CH4Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //
	//
    HAL_TIM_PWM_ConfigChannel(&TIM4_Handler,&TIM4_CH1Handler,TIM_CHANNEL_1);//配置TIM1通道1
    HAL_TIM_PWM_Start(&TIM4_Handler,TIM_CHANNEL_1);//开启PWM通道1
		HAL_TIM_PWM_ConfigChannel(&TIM4_Handler,&TIM4_CH2Handler,TIM_CHANNEL_2);//配置TIM1通道2
    HAL_TIM_PWM_Start(&TIM4_Handler,TIM_CHANNEL_2);//开启PWM通道2
		HAL_TIM_PWM_ConfigChannel(&TIM4_Handler,&TIM4_CH3Handler,TIM_CHANNEL_3);//配置TIM1通道1
    HAL_TIM_PWM_Start(&TIM4_Handler,TIM_CHANNEL_3);//开启PWM通道1
		HAL_TIM_PWM_ConfigChannel(&TIM4_Handler,&TIM4_CH4Handler,TIM_CHANNEL_4);//配置TIM1通道2
    HAL_TIM_PWM_Start(&TIM4_Handler,TIM_CHANNEL_4);//开启PWM通道2
}


void TIM12_PWM_Init(u16 arr,u16 psc)
{ 
    TIM12_Handler.Instance=TIM12;            //定时器3
    TIM12_Handler.Init.Prescaler=psc;       //定时器分频
    TIM12_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM12_Handler.Init.Period=arr;          //自动重装载值
    TIM12_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	

    HAL_TIM_PWM_Init(&TIM12_Handler);       //初始化PWM
    
	//对应 User_PWM_SetPulse
    TIM12_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM12_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM12_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 

		TIM12_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM12_CH2Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM12_CH2Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	
	  TIM12_CH3Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM12_CH3Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM12_CH3Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 

		TIM12_CH4Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM12_CH4Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM12_CH4Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	
    HAL_TIM_PWM_ConfigChannel(&TIM12_Handler,&TIM12_CH1Handler,TIM_CHANNEL_1);//配置TIM1通道1
    HAL_TIM_PWM_Start(&TIM12_Handler,TIM_CHANNEL_1);//开启PWM通道1
		HAL_TIM_PWM_ConfigChannel(&TIM12_Handler,&TIM12_CH2Handler,TIM_CHANNEL_2);//配置TIM1通道2
    HAL_TIM_PWM_Start(&TIM12_Handler,TIM_CHANNEL_2);//开启PWM通道3
		HAL_TIM_PWM_ConfigChannel(&TIM12_Handler,&TIM12_CH3Handler,TIM_CHANNEL_3);//配置TIM1通道1
    HAL_TIM_PWM_Start(&TIM12_Handler,TIM_CHANNEL_3);//开启PWM通道1
		HAL_TIM_PWM_ConfigChannel(&TIM12_Handler,&TIM12_CH4Handler,TIM_CHANNEL_4);//配置TIM1通道2
    HAL_TIM_PWM_Start(&TIM12_Handler,TIM_CHANNEL_4);//开启PWM通道3
}

void TIM15_PWM_Init(u16 arr,u16 psc)
{ 
    TIM15_Handler.Instance=TIM15;            //定时器3
    TIM15_Handler.Init.Prescaler=psc;       //定时器分频
    TIM15_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM15_Handler.Init.Period=arr;          //自动重装载值
    TIM15_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	

    HAL_TIM_PWM_Init(&TIM15_Handler);       //初始化PWM
    
	//对应 User_PWM_SetPulse
    TIM15_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM15_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM15_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 

		TIM15_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM15_CH2Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM15_CH2Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	
    HAL_TIM_PWM_ConfigChannel(&TIM15_Handler,&TIM15_CH1Handler,TIM_CHANNEL_1);//配置TIM1通道1
    HAL_TIM_PWM_Start(&TIM15_Handler,TIM_CHANNEL_1);//开启PWM通道1
		HAL_TIM_PWM_ConfigChannel(&TIM15_Handler,&TIM15_CH2Handler,TIM_CHANNEL_2);//配置TIM1通道2
    HAL_TIM_PWM_Start(&TIM15_Handler,TIM_CHANNEL_2);//开启PWM通道3
}

void TIM16_PWM_Init(u16 arr,u16 psc)
{ 
    TIM16_Handler.Instance=TIM16;            //定时器3
    TIM16_Handler.Init.Prescaler=psc;       //定时器分频
    TIM16_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM16_Handler.Init.Period=arr;          //自动重装载值
    TIM16_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	

    HAL_TIM_PWM_Init(&TIM16_Handler);       //初始化PWM
    
	//对应 User_PWM_SetPulse
    TIM16_CH1Handler.OCMode=TIM_OCMODE_PWM2; //模式选择PWM1
    TIM16_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，//默认比较值为自动重装载值的一半,即占空比为50%
    TIM16_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
	
    HAL_TIM_PWM_ConfigChannel(&TIM16_Handler,&TIM16_CH1Handler,TIM_CHANNEL_1);//配置TIM1通道1
    HAL_TIM_PWM_Start(&TIM16_Handler,TIM_CHANNEL_1);//开启PWM通道1
}


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM1)
	{	
		//msp
		GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_TIM1_CLK_ENABLE();			//使能定时器3
		__HAL_RCC_GPIOE_CLK_ENABLE();			//开启GPIOB时钟
		
	/**TIM1 GPIO Configuration    
		PE9      ------> TIM1_CH1
		PE11     ------> TIM1_CH2
		PE13     ------> TIM1_CH3
		PE14     ------> TIM1_CH4 
    */
		GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_14;;     	
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
		GPIO_Initure.Pull=GPIO_NOPULL;         //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_LOW;     //低速
		GPIO_Initure.Alternate=GPIO_AF1_TIM1;	//PE1复用为TIM3_CH1
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);

	}
	else if(htim->Instance==TIM3)
	{	
		//msp
		GPIO_InitTypeDef GPIO_Initure;
		GPIO_InitTypeDef GPIO_Initure2;
		__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3
		__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
		__HAL_RCC_GPIOA_CLK_ENABLE();	
		
//		PB0     ------> TIM3_CH3
//    PB1     ------> TIM3_CH4
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;     	//PB1
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
		GPIO_Initure.Pull=GPIO_NOPULL;         //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_LOW;     //低速
		GPIO_Initure.Alternate=GPIO_AF2_TIM3;	//PB1复用为TIM3_CH4
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
//		PA6 		------>TIM3_CH1
//		PA7 		------>TIM3_CH2
		GPIO_Initure2.Pin=GPIO_PIN_6|GPIO_PIN_7;     	//
		GPIO_Initure2.Mode=GPIO_MODE_AF_PP;  	//
		GPIO_Initure2.Pull=GPIO_NOPULL;         
		GPIO_Initure2.Speed=GPIO_SPEED_FREQ_LOW;     //低速
		GPIO_Initure2.Alternate=GPIO_AF2_TIM3;	//PB1复用为TIM3_CH4
		HAL_GPIO_Init(GPIOA,&GPIO_Initure2);
		
		
		//    PB4 (NJTRST)     ------> TIM3_CH1
		//	  PB5     ------> TIM3_CH2 

	}
	else if(htim->Instance==TIM4)
	{	
		//msp
		GPIO_InitTypeDef GPIO_Initure;
		GPIO_InitTypeDef GPIO_Initure2;
		__HAL_RCC_TIM4_CLK_ENABLE();			//使能定时器3
		__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
		__HAL_RCC_GPIOD_CLK_ENABLE();			//开启GPIOB时钟
//		    PB6     ------> TIM4_CH1
//        PB7     ------> TIM4_CH2 
		GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;    	//PB1
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
		GPIO_Initure.Pull=GPIO_NOPULL;         //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_LOW;     //低速
		GPIO_Initure.Alternate=GPIO_AF2_TIM4;	//PB1复用为TIM3_CH4
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
//		    PD14     ------> TIM4_CH3
//        PD15     ------> TIM4_CH4 
		GPIO_Initure2.Pin=GPIO_PIN_14|GPIO_PIN_15;    	//PB1
		GPIO_Initure2.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
		GPIO_Initure2.Pull=GPIO_NOPULL;         //上拉
		GPIO_Initure2.Speed=GPIO_SPEED_FREQ_LOW;     //低速
		GPIO_Initure2.Alternate=GPIO_AF2_TIM4;	//PB1复用为TIM3_CH4
		HAL_GPIO_Init(GPIOD,&GPIO_Initure2);

	}
	
		else if(htim->Instance==TIM12)
	{	
		//msp
		GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_TIM12_CLK_ENABLE();			//使能定时器3
		__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
//	    PB14     ------> TIM15_CH1
//	    PB15     ------> TIM15_CH2 
		GPIO_Initure.Pin=GPIO_PIN_14|GPIO_PIN_15;    	//PB1
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
		GPIO_Initure.Pull=GPIO_NOPULL;         //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_LOW;     //低速
		GPIO_Initure.Alternate=GPIO_AF2_TIM12;	//PB1复用为TIM3_CH4
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	}
	
	else if(htim->Instance==TIM15)
	{	
		//msp
		GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_TIM15_CLK_ENABLE();			//使能定时器3
		__HAL_RCC_GPIOE_CLK_ENABLE();			//开启GPIOB时钟
//	    PE5     ------> TIM15_CH1
//	    PE6     ------> TIM15_CH2 
		GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6;    	//PB1
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
		GPIO_Initure.Pull=GPIO_NOPULL;         //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_LOW;     //低速
		GPIO_Initure.Alternate=GPIO_AF4_TIM15;	//PB1复用为TIM3_CH4
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);

	}
		else if(htim->Instance==TIM16)
	{	
		//msp
		GPIO_InitTypeDef GPIO_Initure;
		__HAL_RCC_TIM16_CLK_ENABLE();			//使能定时器3
		__HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
//	    PB8     ------> TIM16_CH1
		GPIO_Initure.Pin=GPIO_PIN_8;    	//PB1
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
		GPIO_Initure.Pull=GPIO_NOPULL;         //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_LOW;     //低速
		GPIO_Initure.Alternate=GPIO_AF1_TIM16;	//PB1复用为TIM3_CH4
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);

	}
	
}

void TIM_SetCompare1(TIM_HandleTypeDef *htim,u32 compare)	
{
	htim->Instance->CCR1 = compare;
}
void TIM_SetCompare2(TIM_HandleTypeDef *htim,u32 compare)	
{
	htim->Instance->CCR2 = compare;
}
void TIM_SetCompare3(TIM_HandleTypeDef *htim,u32 compare)	
{
	htim->Instance->CCR3 = compare;
}
void TIM_SetCompare4(TIM_HandleTypeDef *htim,u32 compare)	
{
	htim->Instance->CCR4 = compare;
}



//

