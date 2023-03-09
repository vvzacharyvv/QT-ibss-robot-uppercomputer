#include "adc.h"	  
#include "timer.h"	
#include "uart.h"
#include "string.h"
ADC_HandleTypeDef ADC1_Handler;//ADC句柄 

uint16_t adc1;
uint16_t adc2;
uint16_t adc3;
uint16_t adc4;
uint16_t adc5;

uint8_t adc_1_H;
uint8_t adc_1_L;
uint8_t adc_2_H;
uint8_t adc_2_L;
uint8_t adc_3_H;
uint8_t adc_3_L;
uint8_t adc_4_H;
uint8_t adc_4_L;
uint8_t adc_5_H;
uint8_t adc_5_L;
uint8_t aa=1;

char c[20]={0};

void MX_ADC1_Init(void)
{
//  ADC_MultiModeTypeDef multimode = {0};
//  ADC_ChannelConfTypeDef sConfig = {0};

//  ADC1_Handler.Instance = ADC1;
//  ADC1_Handler.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;//fen ping
//  ADC1_Handler.Init.Resolution = ADC_RESOLUTION_16B;//wei shu
//  ADC1_Handler.Init.ScanConvMode = ADC_SCAN_DISABLE;//非扫描模式
//  ADC1_Handler.Init.EOCSelection = ADC_EOC_SINGLE_CONV;//Eoc中断
//  ADC1_Handler.Init.LowPowerAutoWait = DISABLE;
//  ADC1_Handler.Init.ContinuousConvMode = DISABLE;//连续转换模式关闭
//  ADC1_Handler.Init.NbrOfConversion = 1;//不连续采样通道数1
//  ADC1_Handler.Init.DiscontinuousConvMode = DISABLE;
//  
//  ADC1_Handler.Init.ExternalTrigConv = ADC_SOFTWARE_START;//软件触发
//  ADC1_Handler.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;//触发极性
//  ADC1_Handler.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
//  ADC1_Handler.Init.Overrun = ADC_OVR_DATA_PRESERVED;
//  ADC1_Handler.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
//  ADC1_Handler.Init.OversamplingMode = DISABLE;

//	HAL_ADC_Init(&ADC1_Handler);  
//	
//  sConfig.Channel = ADC_CHANNEL_14;
//  sConfig.Rank = ADC_REGULAR_RANK_1;
//  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
//  sConfig.SingleDiff = ADC_SINGLE_ENDED;
//  sConfig.OffsetNumber = ADC_OFFSET_NONE;
//  sConfig.Offset = 0;

    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4; 	//4分频，ADCCLK=PER_CK/4=64/4=16MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_16B;           	//16位模式
    ADC1_Handler.Init.ScanConvMode=DISABLE;                    	//非扫描模式
    ADC1_Handler.Init.EOCSelection=ADC_EOC_SINGLE_CONV;       	//关闭EOC中断
	  ADC1_Handler.Init.LowPowerAutoWait=DISABLE;					//自动低功耗关闭				
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;               //关闭连续转换
    ADC1_Handler.Init.NbrOfConversion=1;                        //1个转换在规则序列中 也就是只转换规则序列1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;            //禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion=0;                    //不连续采样通道数为0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;      //软件触发
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
		ADC1_Handler.Init.BoostMode=ENABLE;							//BOOT模式关闭
		ADC1_Handler.Init.Overrun=ADC_OVR_DATA_OVERWRITTEN;			//有新的数据的死后直接覆盖掉旧数据
		ADC1_Handler.Init.OversamplingMode=DISABLE;					//过采样关闭
		ADC1_Handler.Init.ConversionDataManagement=ADC_CONVERSIONDATA_DR;  //规则通道的数据仅仅保存在DR寄存器里面
    HAL_ADC_Init(&ADC1_Handler);                                 //初始化 

		HAL_ADCEx_Calibration_Start(&ADC1_Handler,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED); //ADC校准
}

uint16_t Get_Adc(ADC_HandleTypeDef *hadc,uint32_t ch)   
{
//	ADC_ChannelConfTypeDef ADC_ChanConf;
//    
//	ADC_ChanConf.Channel=ch;                                   //通道
//	ADC_ChanConf.Rank=ADC_REGULAR_RANK_1;                  	//1个序列
//	ADC_ChanConf.SamplingTime=ADC_SAMPLETIME_64CYCLES_5;      	//采样时间       
//	ADC_ChanConf.SingleDiff=ADC_SINGLE_ENDED;  				//单边采集          		
//	ADC_ChanConf.OffsetNumber=ADC_OFFSET_NONE;             	
//	ADC_ChanConf.Offset=0;   
//	HAL_ADC_ConfigChannel(hadc,&ADC_ChanConf);        //通道配置

//	HAL_ADC_Start(hadc);                               //开启ADC
//	
//	HAL_ADC_PollForConversion(hadc,10);                //轮询转换
//	return (uint16_t)HAL_ADC_GetValue(hadc);	            //返回最近一次ADC规则组的转换结果
		ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //通道
    ADC1_ChanConf.Rank=ADC_REGULAR_RANK_1;                  	//1个序列
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_64CYCLES_5;      	//采样时间       
		ADC1_ChanConf.SingleDiff=ADC_SINGLE_ENDED;  				//单边采集          		
		ADC1_ChanConf.OffsetNumber=ADC_OFFSET_NONE;             	
		ADC1_ChanConf.Offset=0;   
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置

    HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //轮询转换
		return (u16)HAL_ADC_GetValue(&ADC1_Handler);	 
}

uint16_t Get_Adc_Average(ADC_HandleTypeDef *hadc,uint32_t ch,uint8_t times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
//		temp_val+=Get_Adc(hadc,ch);
		temp_val+=Get_Adc(&ADC1_Handler,ch);
		//HAL_Delay(5);
	}
	return temp_val/times;
} 

void Read_All_Ad()
{
		adc1 = Get_Adc_Average(&ADC1_Handler,ADC_CHANNEL_16,AD_AVERAGE);
		adc2 = Get_Adc_Average(&ADC1_Handler,ADC_CHANNEL_17,AD_AVERAGE);
		adc3 = Get_Adc_Average(&ADC1_Handler,ADC_CHANNEL_14,AD_AVERAGE);
		adc4 = Get_Adc_Average(&ADC1_Handler,ADC_CHANNEL_15,AD_AVERAGE);
		adc5 = Get_Adc_Average(&ADC1_Handler,ADC_CHANNEL_18,AD_AVERAGE);
		adc_1_H = adc1>>8;
		adc_1_L = adc1&0xff;
		adc_2_H = adc2>>8;
		adc_2_L = adc2&0xff;
		adc_3_H = adc3>>8;
		adc_3_L = adc3&0xff;
		adc_4_H = adc4>>8;
		adc_4_L = adc4&0xff;
		adc_5_H = adc5>>8;
		adc_5_L = adc5&0xff;	
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hadc->Instance==ADC1)
  {
    __HAL_RCC_ADC12_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC1 GPIO Configuration    
		PA0			------> ADC1_INP16
    PA1     ------> ADC1_INP17
    PA2     ------> ADC1_INP14
    PA3     ------> ADC1_INP15
		PA4     ------> ADC1_INP18
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
  }
			GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOD_CLK_ENABLE();					//开启GPIOD时钟
	
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;			//PB0，1
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		//推挽输出
    GPIO_Initure.Pull=GPIO_NOPULL;         			//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  	//高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);     		//初始化GPIOB.0和GPIOB.1

    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);	//PD0置0
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_RESET);	//PD1置0 
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);	//PD0置0
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);	//PD1置0 

}
float remain_v;
void battery_indicatior(void)
{

	if(adc5>7.5*65536/4/3.3)
	{		
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_RESET);	
	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_RESET);	
	}
	if(adc5<=7.2*65536/4/3.3)
	{		
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);	
		if(adc5<6.85*65536/4/3.3)
		{				
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_1,GPIO_PIN_SET);	

			if(adc5<6.6*65536/4/3.3)
			{					
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);	
				if(adc5<6.4*65536/4/3.3)
				{				
					HAL_GPIO_WritePin(GPIOD,GPIO_PIN_3,GPIO_PIN_SET);	
				}
			}
		}
	}
}
//char* itoa(int num,char* str,int radix)
//{
//	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//	unsigned unum;
//	int i=0,j,k;
//	if(radix==10&&num<0)
//	{
//		unum=(unsigned)-num;
//		str[i++]='-';
//	}
//	else unum=(unsigned)num;
//	do
//	{
//		str[i++]=index[unum%(unsigned)radix];
//		unum/=radix;
//	}while(unum);
//	str[i]='\0';

//	if(str[0]=='-') k=1;
//	else k=0;
//	char temp;
//	for(j=k;j<=(i-1)/2;j++)
//	{
//		temp=str[j];
//		str[j]=str[i-1+k-j];
//		str[i-1+k-j]=temp;
//	}
//	return str;
//}

//void Showadc1(void)
//{			
//	itoa(aa,c,10);
//	HAL_UART_Transmit(&huart7,(uint8_t*)c,sizeof(c),1000);
////	HAL_UART_Transmit(&huart7,(uint8_t*)cc,sizeof(cc),1000);
//	memset(c,0,sizeof(c));
//}
