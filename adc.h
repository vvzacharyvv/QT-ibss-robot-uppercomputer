#ifndef __ADC_H
#define __ADC_H
#include "sys.h"

#define AD_AVERAGE 2

extern ADC_HandleTypeDef ADC1_Handler;//ADC¾ä±ú

extern uint8_t adc_1_H;
extern uint8_t adc_1_L;
extern uint8_t adc_2_H;
extern uint8_t adc_2_L;
extern uint8_t adc_3_H;
extern uint8_t adc_3_L;
extern uint8_t adc_4_H;
extern uint8_t adc_4_L;
extern uint8_t adc_5_H;
extern uint8_t adc_5_L;

extern uint16_t adc1;
extern uint16_t adc2;
extern uint16_t adc3;
extern uint16_t adc4;
extern uint16_t adc5;
extern float remain_v;

void MX_ADC1_Init(void);
void Read_All_Ad(void);
void Showadc1(void);
char* itoa(int num,char* str,int radix);
void battery_indicatior(void);
//void ADC_Configuration(void);

//void ADC1_Init(void);
//void ADC1_DMA1_Init(void);
//void ADC1_ExternalTrig_T1_CC1_Init(void);
//void ADC1_GPIO_Config(void);
#endif 

