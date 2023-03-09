#include "SV.h"
#include "adc.h"
#include "pwm.h"
#include "matrix.h"
#include "timer.h"
#include "uart.h"
#define INTEGRALNUM 6
float target_p;
float P[4] = {0};
float p;
float set_p;
float set_speed;
float speed;
float actual_p;
float actual_p_minus;
float err;//ƫ��ֵ
float err_last;//��һ��ƫ��ֵ
float integral_list[INTEGRALNUM] = {0};
float integral = 0;//����ֵ
float Kp, Ki, Kd;

extern struct CreepMotionControl mc;

u8 sv_flag[4] = {0}; // lf, rf, lh, rh; 0: close, 1: open
u8 sv_plus_flag[5] = {0};
u8 sv_minus_flag[5] = {0};

void update_integral_list(float update_value)
{
	for(int i = 0; i<INTEGRALNUM; i++)
	{
		if(i != INTEGRALNUM-1)
		integral_list[i] = integral_list[i+1];
		else
		integral_list[i] = update_value;
	}
}

void SV_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
	
    __HAL_RCC_GPIOC_CLK_ENABLE();					//����GPIOCʱ��
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|
										 GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|
	                   GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_13;//|GPIO_PIN_12;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		//�������
    GPIO_Initure.Pull=GPIO_NOPULL;         			//����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  	//����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);     		//

    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);	//+rf
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);	//-rf
	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);	//+rh
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);	//-rh
	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);	//+lf
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);	//-lf
	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);	//+lh
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);	//-lh
//		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);	//all+
//		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);	//all- 

		__HAL_RCC_GPIOB_CLK_ENABLE();		//����GPIOBʱ��
		GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
		GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		//�������
    GPIO_Initure.Pull=GPIO_NOPULL;         			//����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_LOW;  	//����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);	//LF 	CLOSE
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);	//RF 	CLOSE
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);	//LH 	CLOSE
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET);	//RH 	CLOSE
}
void SV_ALL_P(void)	//+ ��ѹ->����->�Ѹ�
{
	TIM_SetCompare1(&TIM16_Handler,190);//��ѹ
	LF_P_OPEN;
	RF_P_OPEN;
	LH_P_OPEN;
	RH_P_OPEN;
}
void SV_ALL_N(void)
{
	TIM_SetCompare1(&TIM16_Handler,190);//��ѹ
	LF_N_OPEN;
	RF_N_OPEN;
	LH_N_OPEN;
	RH_N_OPEN;
}
void SV_ALL_CLOSE(void)
{
	//TIM_SetCompare1(&TIM16_Handler,190);//��ѹ
	LF_CLOSE;
	RF_CLOSE;
	LH_CLOSE;
	RH_CLOSE;
	
}

void pid_value_init(void)
{
  Kp = 2;
  Ki = 0.2;
  Kd = 2;
  err = 0;
	err_last = 0;
	integral = 0;
	target_p = 55;
	actual_p_minus=-50;
}

// void SV_ESTIMATE(void)
// {
// 	for(int i=0; i<4; i++)
// 	{
// 		if(sv_flag[i]==1)
// 		{			
// 			P[3] = 80*(3.3*adc1/65536-1.45);
// 			P[1] = 80*(3.3*adc2/65536-1.45);
// 			P[0] = 80*(3.3*adc3/65536-1.45);
// 			P[2] = 80*(3.3*adc4/65536-1.45);
// 			actual_p = P[i];
// 			err = target_p - actual_p;
// 			update_integral_list(err);
// 			for(int i=0; i<INTEGRALNUM; i++)
// 			{
// 				integral += integral_list[i];
// 			}
// 			p = Kp * err + Ki*integral + Kd*(err - err_last);
// 			err_last = err;
// 			actual_p = p + actual_p;
// 			speed = 1.5*actual_p-5;
// 			TIM_SetCompare1(&TIM16_Handler,speed);
// 			integral = 0;
// 			break;
// 		}
// 	}
// }

// void SV_ESTIMATE_minus(void)
// {
// 	for(int i=0; i<4; i++)
// 	{
// 		if(sv_flag[i]==1)
// 		{			
// 			P[3] = 80*(3.3*adc1/65536-1.45);
// 			P[1] = 80*(3.3*adc2/65536-1.45);
// 			P[0] = 80*(3.3*adc3/65536-1.45);
// 			P[2] = 80*(3.3*adc4/65536-1.45);
// 			actual_p = P[i];
			
// 			err = actual_p_minus - actual_p;
// 			update_integral_list(err);
// 			for(int i=0; i<INTEGRALNUM; i++)
// 			{
// 				integral += integral_list[i];
// 			}
// 			p = Kp * err + Ki*integral + Kd*(err - err_last);
// 			err_last = err;
// 			actual_p = p + actual_p;
// 			speed = -1.5*actual_p+5;
// 			TIM_SetCompare1(&TIM16_Handler,speed);
// 			integral = 0;
// 			break;
// 		}
// 	}
// }

void air_control_trot(struct_MC *p)
{
	float t = fabs(p->timeForSwingPhase.element[3][0] - p->timeForSwingPhase.element[1][1]) /2.0;		//	harf of minimum time for all feet in stance phase
	// Read_All_Ad();
	// SV_ESTIMATE();
	TIM_SetCompare1(&TIM16_Handler,200);
	if(mc.presentTime < 0)
	{	
		SV_ALL_N();
	}
	if(p->presentTime >= p->timeForSwingPhase.element[1][0] - t && p->presentTime < p->timeForSwingPhase.element[1][0] + p->timeForSwing.element[1][0]*0.4)
	{	
		RF_P_OPEN;
		LH_P_OPEN;
		
		RH_N_OPEN;	
		LF_N_OPEN;
	}
	else if(p->presentTime >= p->timeForSwingPhase.element[1][1] - p->timeForSwing.element[1][0]*0.6 && p->presentTime < p->timeForSwingPhase.element[1][1] + t)
	{
		RF_N_OPEN;
		LH_N_OPEN;
	}	
	if(p->presentTime >= p->timeForSwingPhase.element[0][0] - t && p->presentTime < p->timeForSwingPhase.element[0][0] + p->timeForSwing.element[1][0]*0.4)
	{	
		RH_P_OPEN;	
		LF_P_OPEN;
	}
	else if(p->presentTime >= p->timeForSwingPhase.element[0][1] - p->timeForSwing.element[0][0]*0.6 && p->presentTime < p->timeForSwingPhase.element[0][1] + t)
	{
		RH_N_OPEN;	
		LF_N_OPEN;
	}	

	if(p->timeForSwingPhase.element[0][0] - t < 0)
	{
		if(p->presentTime >= p->timeForSwingPhase.element[0][0] - t + p->timeGait && p->presentTime < p->timeGait)
		{
			RF_N_OPEN;
			LH_N_OPEN;
			
			RH_P_OPEN;	
			LF_P_OPEN;
		}
	}
	if(p->timeForSwingPhase.element[1][0] - t < 0)
	{
		if(p->presentTime >= p->timeForSwingPhase.element[1][0] - t + p->timeGait && p->presentTime < p->timeGait)
		{
			RF_P_OPEN;
			LH_P_OPEN;
			
			RH_N_OPEN;	
			LF_N_OPEN;
		}
	}
}


/*		float t_timeForSwingPhase[]={ 13*mc.timeGait/26,	20*mc.timeGait/26		//lf		�ԽǷ�Э��
																					0,	 7*mc.timeGait/26		//rf
																					0,	 7*mc.timeGait/26		//lh
																		13*mc.timeGait/26,	20*mc.timeGait/26};	//rh
mc.timeGait=4
*/
void air_control_trot_T4u(void)//rf rh lf lh
{
	Read_All_Ad();
	//SV_ESTIMATE();
	TIM_SetCompare1(&TIM16_Handler,190);
	if(mc.presentTime<-0.2)
	{	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);	//rf+  close	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);	//rh+		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);	//lf+		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);	//lh+		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);	//all+	
		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);	//rf-  open	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);	//rh-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);	//lf-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);	//lh-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);	//all-				
	}
	else if(mc.presentTime>=-0.2&&mc.presentTime<0)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);	//rf-  close	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);	//rh-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);	//lf-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);	//lh-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);	//all-				
		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);	//rf+  open  ����rf lh
		//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET);	//rh+		
		//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);	//lf+		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);	//lh+		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);	//all+
	}
	if(mc.presentTime >= 0 && mc.presentTime < 7*mc.timeGait/26 )
	{	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);	//rf+  close	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);	//rh+		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);	//lf+		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);	//lh+		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);	//all+	
	}
	else if(mc.presentTime >= 7*mc.timeGait/26 && mc.presentTime < 10*mc.timeGait/26 )
	{	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);	//rf-  open
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);	//lh-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);	//all-		
	}	
	else if(mc.presentTime >= 10*mc.timeGait/26 && mc.presentTime < 13*mc.timeGait/26 )
	{	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);	//rf-  close	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);	//lh-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);	//all-		
		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET);	//rh+		open
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);	//lf+		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);	//all+		
	}
	else if(mc.presentTime >= 13*mc.timeGait/26	&& mc.presentTime < 20*mc.timeGait/26 )
	{	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);	//rh+		close
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);	//lf+		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);	//all+	
	}
	else if(mc.presentTime >= 20*mc.timeGait/26	&& mc.presentTime < 23*mc.timeGait/26 )
	{	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);	//rh-		open
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);	//lf-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);	//all-		
	}
	else if(mc.presentTime >= 23*mc.timeGait/26	&& mc.presentTime < mc.timeGait )
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);	//rh-		close
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);	//lf-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);	//all-		
		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);	//rf+  open  
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);	//lh+		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);	//all+				
	}
}


void air_control_amble(struct_MC *p)
{
	float t;	//	harf of minimum time for all feet in stance phase

	t = fabsf(p->timeForSwingPhase.element[0][0] - p->timeForSwingPhase.element[1][1]) /2.0;
	if(fabsf(p->timeForSwingPhase.element[2][0] - p->timeForSwingPhase.element[1][1]) /2.0 < t)
		t = fabsf(p->timeForSwingPhase.element[2][0] - p->timeForSwingPhase.element[1][1]) /2.0;
	if(fabsf(p->timeForSwingPhase.element[3][0] - p->timeForSwingPhase.element[1][1]) /2.0 < t)
		t = fabsf(p->timeForSwingPhase.element[3][0] - p->timeForSwingPhase.element[1][1]) /2.0;
	
	// Read_All_Ad();
	//SV_ESTIMATE();
	TIM_SetCompare1(&TIM16_Handler,200);
	if(p->presentTime<-0.2)
	{
		SV_ALL_N();
	}
	else if(p->presentTime>=-0.2&&p->presentTime<0)
	{	
		RF_P_OPEN;
	}
	if(p->presentTime >= p->timeForSwingPhase.element[1][0] - t && p->presentTime < p->timeForSwingPhase.element[1][0] + p->timeForSwing.element[1][0]*0.4)
	{
		RF_P_OPEN;

		LH_N_OPEN;		
		RH_N_OPEN;	
		LF_N_OPEN;
	}
	else if(p->presentTime >= p->timeForSwingPhase.element[1][1] - p->timeForSwing.element[1][0]*0.6 && p->presentTime < p->timeForSwingPhase.element[1][1] + t)
	{
		RF_N_OPEN;
	}
	else if(p->presentTime >= p->timeForSwingPhase.element[3][0] - t && p->presentTime < p->timeForSwingPhase.element[3][0] + p->timeForSwing.element[1][0]*0.4)
	{
		RH_P_OPEN;
	}
	else if(p->presentTime >= p->timeForSwingPhase.element[3][1] - p->timeForSwing.element[3][0]*0.6 && p->presentTime < p->timeForSwingPhase.element[3][1] + t)
	{
		RH_N_OPEN;
	}
	else if(p->presentTime >= p->timeForSwingPhase.element[0][0] - t && p->presentTime < p->timeForSwingPhase.element[0][0] + p->timeForSwing.element[1][0]*0.4)
	{
		LF_P_OPEN;
	}
	else if(p->presentTime >= p->timeForSwingPhase.element[0][1] - p->timeForSwing.element[0][0]*0.6 && p->presentTime < p->timeForSwingPhase.element[0][1] + t)
	{
		LF_N_OPEN;
	}
	else if(p->presentTime >= p->timeForSwingPhase.element[2][0] - t && p->presentTime < p->timeForSwingPhase.element[2][0] + p->timeForSwing.element[1][0]*0.4)
	{
		LH_P_OPEN;
	}
	else if(p->presentTime >= p->timeForSwingPhase.element[2][1] - p->timeForSwing.element[2][0]*0.6 && p->presentTime < p->timeForSwingPhase.element[2][1] + t)
	{
		LH_N_OPEN;
	}

	if( p->timeForSwingPhase.element[0][0] - t < 0)
	{
		if(p->presentTime >= p->timeForSwingPhase.element[0][0] - t + p->timeGait && p->presentTime < mc.timeGait)
		{
			LF_P_OPEN;	
			RF_N_OPEN;
			LH_N_OPEN;	
			RH_N_OPEN;
		}
	}
	else if( p->timeForSwingPhase.element[1][0] - t < 0)
	{
		if(p->presentTime >= p->timeForSwingPhase.element[1][0] - t + p->timeGait && p->presentTime < mc.timeGait)
		{
			LF_N_OPEN;	
			RF_P_OPEN;
			LH_N_OPEN;	
			RH_N_OPEN;
		}
	}
	else if( p->timeForSwingPhase.element[2][0] - t < 0)
	{
		if(p->presentTime >= p->timeForSwingPhase.element[2][0] - t + p->timeGait && p->presentTime < mc.timeGait)
		{
			LF_N_OPEN;	
			RF_N_OPEN;
			LH_P_OPEN;	
			RH_N_OPEN;
		}
	}
	else 	if( p->timeForSwingPhase.element[3][0] - t < 0)
	{
		if(p->presentTime >= p->timeForSwingPhase.element[3][0] - t + p->timeGait && p->presentTime < mc.timeGait)
		{
			LF_N_OPEN;	
			RF_N_OPEN;
			LH_N_OPEN;	
			RH_P_OPEN;
		}
	}
}

//		float t_timeForSwingPhase[]={ 	6*mc.timeGait/12, 	7*mc.timeGait/12,		//lf	���Ƿ�Э��
//																			0*mc.timeGait, 		 	mc.timeGait/12,		//rf
//																		9*mc.timeGait/12,  10*mc.timeGait/12,		//lh
//																		3*mc.timeGait/12, 	4*mc.timeGait/12,};	//rh		
void air_control_amble_T4u(void)
{
	Read_All_Ad();
	//SV_ESTIMATE();
	TIM_SetCompare1(&TIM16_Handler,200);
	if(mc.presentTime<-0.2)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);	//rf+  close	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);	//rh+		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);	//lf+		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);	//lh+		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);	//all+	
		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);	//rf-  open	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);	//rh-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);	//lf-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);	//lh-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);	//all-		
	}
	else if(mc.presentTime>=-0.2&&mc.presentTime<0)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);	//rf-  close	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);	//rh-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);	//lf-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);	//lh-		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);	//all-		

		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET);	//rf+
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET); //all+		
	}
	if(mc.presentTime>=0&&mc.presentTime<mc.timeGait/12)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);	//rf+ close	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET); //all+
	}
	else if(mc.presentTime>=mc.timeGait/12&&mc.presentTime<2*mc.timeGait/12)
	{		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);	//rf-  open	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET); //all-
	}
	else if(mc.presentTime>=2*mc.timeGait/12&&mc.presentTime<3*mc.timeGait/12)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);

		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET); //rh+		open	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET); //all+
	}
	else if(mc.presentTime>=3*mc.timeGait/12&&mc.presentTime<4*mc.timeGait/12)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
	}
	else if(mc.presentTime>=4*mc.timeGait/12&&mc.presentTime<5*mc.timeGait/12)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);	//rh-	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET); 		
	}
	else if(mc.presentTime>=5*mc.timeGait/12&&mc.presentTime<6*mc.timeGait/12)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);

		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET); //lf+	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
	}
	else if(mc.presentTime>=6*mc.timeGait/12&&mc.presentTime<7*mc.timeGait/12)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
	}
	else if(mc.presentTime>=7*mc.timeGait/12&&mc.presentTime<8*mc.timeGait/12)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_RESET);	//lf-			
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);	//all-		
	}
	else if(mc.presentTime>=8*mc.timeGait/12&&mc.presentTime<9*mc.timeGait/12)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);		
		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET); //lh+	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
	}
	else if(mc.presentTime>=9*mc.timeGait/12&&mc.presentTime<10*mc.timeGait/12)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);		
	}
	else if(mc.presentTime>=10*mc.timeGait/12&&mc.presentTime<11*mc.timeGait/12)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET); //lh-
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);		
	}
	else if(mc.presentTime>=11*mc.timeGait/12&&mc.presentTime<mc.timeGait)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
		
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET); //rf+
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);		
	}
	
}

