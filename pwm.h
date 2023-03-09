#ifndef _PWM_H
#define _PWM_H
#include "sys.h"
#include "uart.h"


extern TIM_HandleTypeDef TIM1_Handler;  
extern TIM_HandleTypeDef TIM3_Handler;  //¶¨Ê±Æ÷
extern TIM_HandleTypeDef TIM4_Handler; 
extern TIM_HandleTypeDef TIM12_Handler;
extern TIM_HandleTypeDef TIM15_Handler;
extern TIM_HandleTypeDef TIM16_Handler;

extern TIM_OC_InitTypeDef TIM1_CH1Handler;
extern TIM_OC_InitTypeDef TIM1_CH2Handler;
extern TIM_OC_InitTypeDef TIM1_CH3Handler;
extern TIM_OC_InitTypeDef TIM1_CH4Handler; 
extern TIM_OC_InitTypeDef TIM3_CH1Handler;
extern TIM_OC_InitTypeDef TIM3_CH2Handler;
extern TIM_OC_InitTypeDef TIM3_CH3Handler;
extern TIM_OC_InitTypeDef TIM3_CH4Handler; 
extern TIM_OC_InitTypeDef TIM4_CH1Handler;
extern TIM_OC_InitTypeDef TIM4_CH2Handler;
extern TIM_OC_InitTypeDef TIM4_CH3Handler;
extern TIM_OC_InitTypeDef TIM4_CH4Handler;
extern TIM_OC_InitTypeDef TIM12_CH1Handler;
extern TIM_OC_InitTypeDef TIM12_CH2Handler;
extern TIM_OC_InitTypeDef TIM12_CH3Handler;
extern TIM_OC_InitTypeDef TIM12_CH4Handler;

extern TIM_OC_InitTypeDef TIM15_CH1Handler;
extern TIM_OC_InitTypeDef TIM15_CH2Handler; 
extern TIM_OC_InitTypeDef TIM16_CH1Handler;

void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);
void TIM12_PWM_Init(u16 arr,u16 psc);
void TIM15_PWM_Init(u16 arr,u16 psc);
void TIM16_PWM_Init(u16 arr,u16 psc);

//void TIM_SetTIM3Compare4(u32 compare);
void TIM_SetCompare1(TIM_HandleTypeDef *htim,u32 compare);
void TIM_SetCompare2(TIM_HandleTypeDef *htim,u32 compare);
void TIM_SetCompare3(TIM_HandleTypeDef *htim,u32 compare);
void TIM_SetCompare4(TIM_HandleTypeDef *htim,u32 compare);



#endif

