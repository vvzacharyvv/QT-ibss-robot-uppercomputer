#include "ForceSensor.h"
#include "string.h"
#include <math.h>
#include "can.h"
#include "uart.h"

int F20[3]={0};
int F10[3]={0};
int F2[3]={0};
int F1[3]={0};
char cc[3] ="  ";
char c[20]={0};

void GetForce_init(void)
{
	//F20 = {a[0][0],a[0][1],a[0][2]};
	//char c[20]={0};
	//memset(c,0,sizeof(c));
	u8 times = 20;
	for(int i=0;i<times;i++)
	{		
		//	u8 a[3][3]={0};
			u8 b[3][3]={0};
		//	int temp20[3]={0};
			int temp10[3]={0};
			
//			a[0][0] = canbuf1[2];
//			a[0][1] = canbuf1[3];
//			a[0][2] = canbuf1[4];
//			a[1][0] = canbuf1[5];
//			a[1][1] = canbuf1[6];
//			a[1][2] = canbuf1[7];
//			a[2][0] = canbuf2[0];
//			a[2][1] = canbuf2[1];
//			a[2][2] = canbuf2[2];
			
//			b[0][0] = canbuf2[3];
//			b[0][1] = canbuf2[4];
//			b[0][2] = canbuf2[5];
//			b[1][0] = canbuf2[6];
//			b[1][1] = canbuf2[7];
//			b[1][2] = canbuf3[0];
			b[2][0] = canbuf3[1];
			b[2][1] = canbuf3[2];
			b[2][2] = canbuf3[3];
			
//			temp20[0] = (a[0][0]&0x7F)<<16 | a[0][1]<<8 | a[0][2]; //b=temp20  
//			temp20[1] = (a[1][0]&0x7F)<<16 | a[1][1]<<8 | a[1][2]; //b=temp20  
//			temp20[2] = (a[2][0]&0x7F)<<16 | a[2][1]<<8 | a[2][2]; //b=temp20 
//			
//			temp10[0] = (b[0][0]&0x7F)<<16 | b[0][1]<<8 | b[0][2]; //  
//			temp10[1] = (b[1][0]&0x7F)<<16 | b[1][1]<<8 | b[1][2]; //  
			temp10[2] = (b[2][0]&0x7F)<<16 | b[2][1]<<8 | b[2][2]; //
				
//			if((a[0][0]&0x80)==0x80) {temp20[0] = -temp20[0] ;}
//			if((a[1][0]&0x80)==0x80) {temp20[1] = -temp20[1] ;}
//			if((a[2][0]&0x80)==0x80) {temp20[2] = -temp20[2] ;}
//			
//			if((b[0][0]&0x80)==0x80) {temp10[0] = -temp10[0] ;}
//			if((b[1][0]&0x80)==0x80) {temp10[1] = -temp10[1] ;}
			if((b[2][0]&0x80)==0x80) {temp10[2] = -temp10[2] ;}
			
			//itoa(temp10[2],c,10);
			//HAL_UART_Transmit(&huart4,(uint8_t*)c,sizeof(c),1000);
			//HAL_UART_Transmit(&huart4,(uint8_t*)cc,sizeof(cc),1000);
			//memset(c,0,sizeof(c));
						

//		F20[i]=F20[i]+temp20[i];
			F10[2]=F10[2]+temp10[2];	
					
	}

//		F20[i]=F20[i]/times;
			F10[2]=F10[2]/times;	
			
//			itoa(F10[2],c,10);
//			HAL_UART_Transmit(&huart4,(uint8_t*)c,sizeof(c),1000);
//			HAL_UART_Transmit(&huart4,(uint8_t*)cc,sizeof(cc),1000);
			//memset(c,0,sizeof(c));
}	
void GetForce(void)
{		
//			u8 a[3][3]={0};
			u8 b[3][3]={0};
			
//			int temp2[3]={0};
			int temp1[3]={0};
			
//			a[0][0] = canbuf1[2];
//			a[0][1] = canbuf1[3];
//			a[0][2] = canbuf1[4];
//			a[1][0] = canbuf1[5];
//			a[1][1] = canbuf1[6];
//			a[1][2] = canbuf1[7];
//			a[2][0] = canbuf2[0];
//			a[2][1] = canbuf2[1];
//			a[2][2] = canbuf2[2];
			
//			b[0][0] = canbuf2[3];
//			b[0][1] = canbuf2[4];
//			b[0][2] = canbuf2[5];
//			b[1][0] = canbuf2[6];
//			b[1][1] = canbuf2[7];
//			b[1][2] = canbuf3[0];
			b[2][0] = canbuf3[1];
			b[2][1] = canbuf3[2];
			b[2][2] = canbuf3[3];
			
			
//			temp2[0] = (a[0][0]&0x7F)<<16 | a[0][1]<<8 | a[0][2]; //b=temp20  
//			temp2[1] = (a[1][0]&0x7F)<<16 | a[1][1]<<8 | a[1][2]; //b=temp20  
//			temp2[2] = (a[2][0]&0x7F)<<16 | a[2][1]<<8 | a[2][2]; //b=temp20 
			
//			temp1[0] = (b[0][0]&0x7F)<<16 | b[0][1]<<8 | b[0][2]; //  
//			temp1[1] = (b[1][0]&0x7F)<<16 | b[1][1]<<8 | b[1][2]; //  
			temp1[2] = (b[2][0]&0x7F)<<16 | b[2][1]<<8 | b[2][2]; //
			
//			if((a[0][0]&0x80)==0x80) {temp2[0] = -temp2[0] ;}
//			if((a[1][0]&0x80)==0x80) {temp2[1] = -temp2[1] ;}
//			if((a[2][0]&0x80)==0x80) {temp2[2] = -temp2[2] ;}
//			
//			if((b[0][0]&0x80)==0x80) {temp1[0] = -temp1[0] ;}
//			if((b[1][0]&0x80)==0x80) {temp1[1] = -temp1[1] ;}
			if((b[2][0]&0x80)==0x80) {temp1[2] = -temp1[2] ;}
			
			//itoa(temp1[2],c,10);
			//HAL_UART_Transmit(&huart4,(uint8_t*)c,sizeof(c),1000);
			//HAL_UART_Transmit(&huart4,(uint8_t*)cc,sizeof(cc),1000);
			//memset(c,0,sizeof(c));
			
			F1[2] = temp1[2] - F10[2];
	}

char* itoa(int num,char* str,int radix)
{
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	unsigned unum;
	int i=0,j,k;
	if(radix==10&&num<0)
	{
		unum=(unsigned)-num;
		str[i++]='-';
	}
	else unum=(unsigned)num;
	do
	{
		str[i++]=index[unum%(unsigned)radix];
		unum/=radix;
	}while(unum);
	str[i]='\0';

	if(str[0]=='-') k=1;
	else k=0;
	char temp;
	for(j=k;j<=(i-1)/2;j++)
	{
		temp=str[j];
		str[j]=str[i-1+k-j];
		str[i-1+k-j]=temp;
	}
	return str;
}

void ShowF1(void)
{			
	itoa(F1[2],c,10);
	HAL_UART_Transmit(&huart4,(uint8_t*)c,sizeof(c),1000);
	HAL_UART_Transmit(&huart4,(uint8_t*)cc,sizeof(cc),1000);
	memset(c,0,sizeof(c));
}