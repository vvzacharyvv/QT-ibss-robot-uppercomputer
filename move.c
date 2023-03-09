#include <math.h>
#include "pwm.h"	
#include "capture.h"
#include "adc.h"
#include "SV.h"
#include "move.h"
#include <sys.h>
const short L1=38;
const short L2=47;
const short L3=22;

KinematicsArm KMGecko;

u8 step =50;
int i;
float X_o2f[4] = {109, 109,- 109,- 109};  // LF/RF/LH/RH
float Y_o2f[4] = {68.24, -68.24,68.24, -68.24};  // 
float Z_o2f[4]={-22,-22,-22,-22};
float X_o2s[4] = {62,62,-62,-62};  // 
float Y_o2s[4] = {30.24, -30.24,30.24, -30.24}; //
float X_o = 0;
float Y_o = 0;
float Z_o = 0;
float x[4]={0};
float y[4]={0};
float z[4]={0};
//?????
	float p_wox[4];
	float p_woy[4];
	float p_woz[4];
	//??????
	float p_osx[4];
	float p_osy[4];
	float p_osz[4];
	//?????
	float p_wfx[4];
	float p_wfy[4];
	float p_wfz[4];

	float L_onestep = 20.0;           // ???????????
	float H_onestep = 10.0;           // ???????????
	float v_x;                        // ?????????
	float v_body;                     // ??????
  float t = 0;
	float T=3;
float cx[4]={0};
float cy[4]={0};
float cz[4]={0};
//void setcurrentposition(int ft,float x,float y,float z)
//{
//	// Try not to use too many if... in one function, replace them with switch
//	if (ft==LF)
//	{cx[0]=x;
//	 cy[0]=y;
//	 cz[0]=z;
//	}
//	
//	if (ft==RF)
//	{cx[1]=x;
//	 cy[1]=y;
//	 cz[1]=z;
//	}
//	
//	if (ft==LH)
//	{cx[2]=x;
//	 cy[2]=y;
//	 cz[2]=z;
//	}

//	if (ft==RH)
//	{cx[3]=x;
//	 cy[3]=y;
//	 cz[3]=z;
//	}

//}
void movement(void)
{
	float a[4][3] = {0};     //   a1 = a[0]   a2=a[LF]
	float theta[4][2] = {0};
		int s=1;
		while(t<T/4)
		{			
				t=s*T/4/step;
				x[0]=47-L_onestep*s/step/4;
				y[0]=38;
				z[0]=-22;
				x[1]=47+L_onestep*s*3/step/4;
				y[1]=-38;
				z[1]=-22+H_onestep-H_onestep*(L_onestep*s*3/step/4- L_onestep/8*3)* (L_onestep*s*3/step/4- L_onestep/8*3)/(L_onestep/8*3*L_onestep/8*3);
				x[2]=47-L_onestep*s/step/4;
				y[2]=38;
				z[2]=-22;
				x[3]=47-L_onestep*s/step/4;
				y[3]=38;
				z[3]=-22;
				for (i=0;i<4;i++)
				{
					a[i][0] = asin(-L3 / sqrt(y[i] * y[i] + z[i] * z[i]) ) - atan2(z[i],-y[i]);
					a[i][1] = asin((y[i] * y[i] + x[i] * x[i] + z[i] * z[i] + L1 * L1 - L2 * L2 - L3 * L3) / ( 2 * L1 * sqrt (y[i] * y[i] +  x[i] * x[i] + z[i] * z[i] - L3 * L3)) )
												- atan2(sqrt(y[i] * y[i] + z[i] * z[i] - L3 * L3) , x[i]);
					a[i][2] = asin((L1 * L1 + L2 * L2 + L3 * L3 - y[i] * y[i] - x[i] * x[i] - z[i] * z[i]) / (2 * L1 * L2));
				}
					theta[0][0]=(a[0][0]+a[0][1]);
			    theta[0][1]=(a[0][1]-a[0][0]);
					
					theta[0][0]=(int) (theta[0][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[0][1]=(int) (theta[0][1]/3.1415926*180*100);
					a[0][2]=(int) (a[0][2]/3.1415926*180*100);

					theta[0][0]= (float) (theta[0][0]/100);
					theta[0][1]= (float) (theta[0][1]/100);
					a[0][2]= (float) (a[0][2]/100);
				  
				  theta[1][0]=(a[1][0]+a[1][1]);
					theta[1][1]=(a[1][1]-a[1][0]);
					
					theta[1][0]=(int) (theta[1][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[1][1]=(int) (theta[1][1]/3.1415926*180*100);
					a[1][2]=(int) (a[1][2]/3.1415926*180*100);
				
				  theta[1][0]= (float) (-theta[1][0]/100);
					theta[1][1]= (float) (-theta[1][1]/100);
					a[1][2]= (float) (-a[1][2]/100);

				  theta[2][0]=(a[2][1]-a[2][0]);
					theta[2][1]=(a[2][1]+a[2][0]);
					
					theta[2][0]=(int) (theta[2][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[2][1]=(int) (theta[2][1]/3.1415926*180*100);
					a[2][2]=(int) (a[2][2]/3.1415926*180*100);

					theta[2][0]= (float) (theta[2][0]/100);
					theta[2][1]= (float) (theta[2][1]/100);
					a[2][2]= (float) (a[2][2]/100);
					
					theta[3][0]=(a[3][1]-a[3][0]);
					theta[3][1]=(a[3][1]+a[3][0]);
					
					theta[3][0]=(int) (theta[3][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[3][1]=(int) (theta[3][1]/3.1415926*180*100);
					a[3][2]=(int) (a[3][2]/3.1415926*180*100);

					theta[3][0]= (float) (-theta[3][0]/100);
					theta[3][1]= (float) (-theta[3][1]/100);
					a[3][2]= (float) (-a[3][2]/100);
				
					angle_f(theta[0][0]+KMGecko.StartAngle[LF_J3],LF_J3);
					angle_f(theta[0][1]+KMGecko.StartAngle[LF_J2],LF_J2);
					angle_f(a[0][2]+KMGecko.StartAngle[LF_J1],LF_J1);
						
					angle_f(theta[1][0]+KMGecko.StartAngle[RF_J3],RF_J3);
					angle_f(theta[1][1]+KMGecko.StartAngle[RF_J2],RF_J2);
					angle_f(a[1][2]+KMGecko.StartAngle[RF_J1],RF_J1);
						
					angle_f(theta[2][0]+KMGecko.StartAngle[LH_J3],LH_J3);
					angle_f(theta[2][1]+KMGecko.StartAngle[LH_J2],LH_J2);
					angle_f(a[2][2]+KMGecko.StartAngle[LH_J1],LH_J1);
						
					angle_f(theta[3][0]+KMGecko.StartAngle[RH_J3],RH_J3);
					angle_f(theta[3][1]+KMGecko.StartAngle[RH_J2],RH_J2);
					angle_f(a[3][2]+KMGecko.StartAngle[RH_J1],RH_J1);

					s=s+1;

		}
		while(t>=T/4&&t<T/2)
		{
			
				t=s*T/4/step;
				x[0]=47-L_onestep*s/step/4;
				y[0]=38;
				z[0]=-22;
				x[1]=47-L_onestep*s/step/4+L_onestep;
				y[1]=-38;
				z[1]=-22;
				x[2]=47-L_onestep*s/step/4;
				y[2]=38;
				z[2]=-22;
				x[3]=47+L_onestep*(s-step)*3/step/4-L_onestep/4;
				y[3]=38;
				z[3]=-22+H_onestep-H_onestep*(L_onestep*(s-step)*3/step/4- L_onestep/8*3)* (L_onestep*(s-step)*3/step/4- L_onestep/8*3)/(L_onestep/8*3*L_onestep/8*3);
		for (i=0;i<4;i++)
				{
					a[i][0] = asin(-L3 / sqrt(y[i] * y[i] + z[i] * z[i]) ) - atan2(z[i],-y[i]);
					a[i][1] = asin((y[i] * y[i] + x[i] * x[i] + z[i] * z[i] + L1 * L1 - L2 * L2 - L3 * L3) / ( 2 * L1 * sqrt (y[i] * y[i] +  x[i] * x[i] + z[i] * z[i] - L3 * L3)) )
												- atan2(sqrt(y[i] * y[i] + z[i] * z[i] - L3 * L3) , x[i]);
					a[i][2] = asin((L1 * L1 + L2 * L2 + L3 * L3 - y[i] * y[i] - x[i] * x[i] - z[i] * z[i]) / (2 * L1 * L2));
				}
					theta[0][0]=(a[0][0]+a[0][1]);
			    theta[0][1]=(a[0][1]-a[0][0]);
					
					theta[0][0]=(int) (theta[0][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[0][1]=(int) (theta[0][1]/3.1415926*180*100);
					a[0][2]=(int) (a[0][2]/3.1415926*180*100);

					theta[0][0]= (float) (theta[0][0]/100);
					theta[0][1]= (float) (theta[0][1]/100);
					a[0][2]= (float) (a[0][2]/100);
				  
				  theta[1][0]=(a[1][0]+a[1][1]);
					theta[1][1]=(a[1][1]-a[1][0]);
					
					theta[1][0]=(int) (theta[1][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[1][1]=(int) (theta[1][1]/3.1415926*180*100);
					a[1][2]=(int) (a[1][2]/3.1415926*180*100);
				
				  theta[1][0]= (float) (-theta[1][0]/100);
					theta[1][1]= (float) (-theta[1][1]/100);
					a[1][2]= (float) (-a[1][2]/100);

				  theta[2][0]=(a[2][1]-a[2][0]);
					theta[2][1]=(a[2][1]+a[2][0]);
					
					theta[2][0]=(int) (theta[2][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[2][1]=(int) (theta[2][1]/3.1415926*180*100);
					a[2][2]=(int) (a[2][2]/3.1415926*180*100);

					theta[2][0]= (float) (theta[2][0]/100);
					theta[2][1]= (float) (theta[2][1]/100);
					a[2][2]= (float) (a[2][2]/100);
					
					theta[3][0]=(a[3][1]-a[3][0]);
					theta[3][1]=(a[3][1]+a[3][0]);
					
					theta[3][0]=(int) (theta[3][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[3][1]=(int) (theta[3][1]/3.1415926*180*100);
					a[3][2]=(int) (a[3][2]/3.1415926*180*100);

					theta[3][0]= (float) (-theta[3][0]/100);
					theta[3][1]= (float) (-theta[3][1]/100);
					a[3][2]= (float) (-a[3][2]/100);
				
					angle_f(theta[0][0]+KMGecko.StartAngle[LF_J3],LF_J3);
					angle_f(theta[0][1]+KMGecko.StartAngle[LF_J2],LF_J2);
					angle_f(a[0][2]+KMGecko.StartAngle[LF_J1],LF_J1);
						
					angle_f(theta[1][0]+KMGecko.StartAngle[RF_J3],RF_J3);
					angle_f(theta[1][1]+KMGecko.StartAngle[RF_J2],RF_J2);
					angle_f(a[1][2]+KMGecko.StartAngle[RF_J1],RF_J1);
						
					angle_f(theta[2][0]+KMGecko.StartAngle[LH_J3],LH_J3);
					angle_f(theta[2][1]+KMGecko.StartAngle[LH_J2],LH_J2);
					angle_f(a[2][2]+KMGecko.StartAngle[LH_J1],LH_J1);
						
					angle_f(theta[3][0]+KMGecko.StartAngle[RH_J3],RH_J3);
					angle_f(theta[3][1]+KMGecko.StartAngle[RH_J2],RH_J2);
					angle_f(a[3][2]+KMGecko.StartAngle[RH_J1],RH_J1);
		s=s+1;
		
		}
		while(t>=T/2&&t<T*3/4)
		{

				t=s*T/4/step;
				x[0]=47+L_onestep*(s-2*step)*3/step/4-L_onestep/2;
				y[0]=38;
				z[0]=-22+H_onestep-H_onestep*(L_onestep*(s-2*step)*3/step/4- L_onestep/8*3)* (L_onestep*(s-2*step)*3/step/4- L_onestep/8*3)/(L_onestep/8*3*L_onestep/8*3);
				x[1]=47-L_onestep*s/step/4+L_onestep;
				y[1]=-38;
				z[1]=-22;
				x[2]=47-L_onestep*s/step/4;
				y[2]=38;
				z[2]=-22;
				x[3]=47-L_onestep*s/step/4+L_onestep;
				y[3]=38;
				z[3]=-22;
			for (i=0;i<4;i++)
				{
					a[i][0] = asin(-L3 / sqrt(y[i] * y[i] + z[i] * z[i]) ) - atan2(z[i],-y[i]);
					a[i][1] = asin((y[i] * y[i] + x[i] * x[i] + z[i] * z[i] + L1 * L1 - L2 * L2 - L3 * L3) / ( 2 * L1 * sqrt (y[i] * y[i] +  x[i] * x[i] + z[i] * z[i] - L3 * L3)) )
												- atan2(sqrt(y[i] * y[i] + z[i] * z[i] - L3 * L3) , x[i]);
					a[i][2] = asin((L1 * L1 + L2 * L2 + L3 * L3 - y[i] * y[i] - x[i] * x[i] - z[i] * z[i]) / (2 * L1 * L2));
				}
					theta[0][0]=(a[0][0]+a[0][1]);
			    theta[0][1]=(a[0][1]-a[0][0]);
					
					theta[0][0]=(int) (theta[0][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[0][1]=(int) (theta[0][1]/3.1415926*180*100);
					a[0][2]=(int) (a[0][2]/3.1415926*180*100);

					theta[0][0]= (float) (theta[0][0]/100);
					theta[0][1]= (float) (theta[0][1]/100);
					a[0][2]= (float) (a[0][2]/100);
				  
				  theta[1][0]=(a[1][0]+a[1][1]);
					theta[1][1]=(a[1][1]-a[1][0]);
					
					theta[1][0]=(int) (theta[1][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[1][1]=(int) (theta[1][1]/3.1415926*180*100);
					a[1][2]=(int) (a[1][2]/3.1415926*180*100);
				
				  theta[1][0]= (float) (-theta[1][0]/100);
					theta[1][1]= (float) (-theta[1][1]/100);
					a[1][2]= (float) (-a[1][2]/100);

				  theta[2][0]=(a[2][1]-a[2][0]);
					theta[2][1]=(a[2][1]+a[2][0]);
					
					theta[2][0]=(int) (theta[2][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[2][1]=(int) (theta[2][1]/3.1415926*180*100);
					a[2][2]=(int) (a[2][2]/3.1415926*180*100);

					theta[2][0]= (float) (theta[2][0]/100);
					theta[2][1]= (float) (theta[2][1]/100);
					a[2][2]= (float) (a[2][2]/100);
					
					theta[3][0]=(a[3][1]-a[3][0]);
					theta[3][1]=(a[3][1]+a[3][0]);
					
					theta[3][0]=(int) (theta[3][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[3][1]=(int) (theta[3][1]/3.1415926*180*100);
					a[3][2]=(int) (a[3][2]/3.1415926*180*100);

					theta[3][0]= (float) (-theta[3][0]/100);
					theta[3][1]= (float) (-theta[3][1]/100);
					a[3][2]= (float) (-a[3][2]/100);
				
					angle_f(theta[0][0]+KMGecko.StartAngle[LF_J3],LF_J3);
					angle_f(theta[0][1]+KMGecko.StartAngle[LF_J2],LF_J2);
					angle_f(a[0][2]+KMGecko.StartAngle[LF_J1],LF_J1);
						
					angle_f(theta[1][0]+KMGecko.StartAngle[RF_J3],RF_J3);
					angle_f(theta[1][1]+KMGecko.StartAngle[RF_J2],RF_J2);
					angle_f(a[1][2]+KMGecko.StartAngle[RF_J1],RF_J1);
						
					angle_f(theta[2][0]+KMGecko.StartAngle[LH_J3],LH_J3);
					angle_f(theta[2][1]+KMGecko.StartAngle[LH_J2],LH_J2);
					angle_f(a[2][2]+KMGecko.StartAngle[LH_J1],LH_J1);
						
					angle_f(theta[3][0]+KMGecko.StartAngle[RH_J3],RH_J3);
					angle_f(theta[3][1]+KMGecko.StartAngle[RH_J2],RH_J2);
					angle_f(a[3][2]+KMGecko.StartAngle[RH_J1],RH_J1);
		s=s+1;
		}
		while(t>=3*T/4&&t<T)
		{
			
				t=s*T/4/step;
				x[0]=47-L_onestep*s/step/4+L_onestep;
				y[0]=38;
				z[0]=-22;
				x[1]=47-L_onestep*s/step/4+L_onestep;
				y[1]=-38;
				z[1]=-22;
				x[2]=47+L_onestep*(s-3*step)*3/step/4-L_onestep*3/4;
				y[2]=38;
				z[2]=-22+H_onestep-H_onestep*(L_onestep*(s-3*step)*3/step/4- L_onestep/8*3)* (L_onestep*(s-3*step)*3/step/4- L_onestep/8*3)/(L_onestep/8*3*L_onestep/8*3);
				x[3]=47-L_onestep*s/step/4+L_onestep;
				y[3]=38;
				z[3]=-22;
			for (i=0;i<4;i++)
				{
					a[i][0] = asin(-L3 / sqrt(y[i] * y[i] + z[i] * z[i]) ) - atan2(z[i],-y[i]);
					a[i][1] = asin((y[i] * y[i] + x[i] * x[i] + z[i] * z[i] + L1 * L1 - L2 * L2 - L3 * L3) / ( 2 * L1 * sqrt (y[i] * y[i] +  x[i] * x[i] + z[i] * z[i] - L3 * L3)) )
												- atan2(sqrt(y[i] * y[i] + z[i] * z[i] - L3 * L3) , x[i]);
					a[i][2] = asin((L1 * L1 + L2 * L2 + L3 * L3 - y[i] * y[i] - x[i] * x[i] - z[i] * z[i]) / (2 * L1 * L2));
				}
					theta[0][0]=(a[0][0]+a[0][1]);
			    theta[0][1]=(a[0][1]-a[0][0]);
					
					theta[0][0]=(int) (theta[0][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[0][1]=(int) (theta[0][1]/3.1415926*180*100);
					a[0][2]=(int) (a[0][2]/3.1415926*180*100);

					theta[0][0]= (float) (theta[0][0]/100);
					theta[0][1]= (float) (theta[0][1]/100);
					a[0][2]= (float) (a[0][2]/100);
				  
				  theta[1][0]=(a[1][0]+a[1][1]);
					theta[1][1]=(a[1][1]-a[1][0]);
					
					theta[1][0]=(int) (theta[1][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[1][1]=(int) (theta[1][1]/3.1415926*180*100);
					a[1][2]=(int) (a[1][2]/3.1415926*180*100);
				
				  theta[1][0]= (float) (-theta[1][0]/100);
					theta[1][1]= (float) (-theta[1][1]/100);
					a[1][2]= (float) (-a[1][2]/100);

				  theta[2][0]=(a[2][1]-a[2][0]);
					theta[2][1]=(a[2][1]+a[2][0]);
					
					theta[2][0]=(int) (theta[2][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[2][1]=(int) (theta[2][1]/3.1415926*180*100);
					a[2][2]=(int) (a[2][2]/3.1415926*180*100);

					theta[2][0]= (float) (theta[2][0]/100);
					theta[2][1]= (float) (theta[2][1]/100);
					a[2][2]= (float) (a[2][2]/100);
					
					theta[3][0]=(a[3][1]-a[3][0]);
					theta[3][1]=(a[3][1]+a[3][0]);
					
					theta[3][0]=(int) (theta[3][0]/3.1415926*180*100);//rf alpha 角，最内侧
					theta[3][1]=(int) (theta[3][1]/3.1415926*180*100);
					a[3][2]=(int) (a[3][2]/3.1415926*180*100);

					theta[3][0]= (float) (-theta[3][0]/100);
					theta[3][1]= (float) (-theta[3][1]/100);
					a[3][2]= (float) (-a[3][2]/100);
				
					angle_f(theta[0][0]+KMGecko.StartAngle[LF_J3],LF_J3);
					angle_f(theta[0][1]+KMGecko.StartAngle[LF_J2],LF_J2);
					angle_f(a[0][2]+KMGecko.StartAngle[LF_J1],LF_J1);
						
					angle_f(theta[1][0]+KMGecko.StartAngle[RF_J3],RF_J3);
					angle_f(theta[1][1]+KMGecko.StartAngle[RF_J2],RF_J2);
					angle_f(a[1][2]+KMGecko.StartAngle[RF_J1],RF_J1);
						
					angle_f(theta[2][0]+KMGecko.StartAngle[LH_J3],LH_J3);
					angle_f(theta[2][1]+KMGecko.StartAngle[LH_J2],LH_J2);
					angle_f(a[2][2]+KMGecko.StartAngle[LH_J1],LH_J1);
						
					angle_f(theta[3][0]+KMGecko.StartAngle[RH_J3],RH_J3);
					angle_f(theta[3][1]+KMGecko.StartAngle[RH_J2],RH_J2);
					angle_f(a[3][2]+KMGecko.StartAngle[RH_J1],RH_J1);
				
		s=s+1;
		}
	}
void move(void)
{
movement();
}
void StartAngleInit(void)
{
			/**直角初始位置**/
	//关节1、2、3分别为踝关节、膝关节、髋关节
	//number 3
	KMGecko.StartAngle[RF_J3] = -5; //52
	KMGecko.StartAngle[RF_J2] = 5; //26//large figure turning to the inside
	KMGecko.StartAngle[RF_J1] = 0; //// large figure turning to the inside
   //number 2
	KMGecko.StartAngle[RH_J3] = 5;//-10//-25;	
	KMGecko.StartAngle[RH_J2] = -5;//50;	small figure clockwise
	KMGecko.StartAngle[RH_J1] = 0 ;//30;small figure turning to the clw
  //number 0
	KMGecko.StartAngle[LF_J3] = 5;//-10
	KMGecko.StartAngle[LF_J2] = -5;//-13 small figure turning to the inside
	KMGecko.StartAngle[LF_J1] = 0;//-58 small figure turning to the inside
	//number 1
	KMGecko.StartAngle[LH_J3] = -5;//10///-30;//原值为-30
	KMGecko.StartAngle[LH_J2] = 5;//0 large figure turning to the inside
	KMGecko.StartAngle[LH_J1] = 0;//;small figure turning to the inside
}

void InitRobotPosion(void)
{
//	if(((Time8Channel3HighTime<=1600)&&(Time8Channel3HighTime>=1400))||(Time8Channel3HighTime<=500))
//	{	
	
	angle_f(KMGecko.StartAngle[LF_J1],LF_J1);
	angle_f(KMGecko.StartAngle[LF_J2],LF_J2);
	angle_f(KMGecko.StartAngle[LF_J3],LF_J3);
	
	angle_f(KMGecko.StartAngle[RF_J1],RF_J1);
	angle_f(KMGecko.StartAngle[RF_J2],RF_J2);		
	angle_f(KMGecko.StartAngle[RF_J3],RF_J3);
	
	angle_f(KMGecko.StartAngle[RH_J1],RH_J1);
	angle_f(KMGecko.StartAngle[RH_J2],RH_J2);
	angle_f(KMGecko.StartAngle[RH_J3],RH_J3);

	angle_f(KMGecko.StartAngle[LH_J1],LH_J1);
	angle_f(KMGecko.StartAngle[LH_J2],LH_J2);
	angle_f(KMGecko.StartAngle[LH_J3],LH_J3);
	
//	setcurrentposition(RF,L1,L2,-L3);
//	setcurrentposition(RH,L1,L2,-L3);
//	setcurrentposition(LF,L1,L2,-L3);
//	setcurrentposition(LH,L1,L2,-L3);
//	HAL_Delay(30);	
}

void angle_f(float angle,int footnumber)
{
	int16_t ArrValue = 0;
	//ArrValue = 250 + (int32_t) (angle *0.18);
	//ArrValue = angle * 5 + 750;
	ArrValue = (int32_t) (angle * 5.56 + 750)	;
	if(footnumber == LF_J1)
	{
	//	User_PWM_SetValue(&htim4, TIM_CHANNEL_2,ArrValue);
		TIM_SetCompare1(&TIM4_Handler,ArrValue);
	}	
	else if(footnumber == LF_J2)
	{
	//	User_PWM_SetValue(&htim4, TIM_CHANNEL_1,ArrValue);
		TIM_SetCompare2(&TIM4_Handler,ArrValue);
	}	
	else if(footnumber == LF_J3)
	{
	//	User_PWM_SetValue(&htim4, TIM_CHANNEL_1,ArrValue);
		TIM_SetCompare3(&TIM4_Handler,ArrValue);
	}	
	else if(footnumber == LH_J1)
	{
	//	User_PWM_SetValue(&htim4, TIM_CHANNEL_1,ArrValue);
		TIM_SetCompare1(&TIM12_Handler,ArrValue);
	}	
	else if(footnumber == LH_J2)
	{
	//	User_PWM_SetValue(&htim4, TIM_CHANNEL_1,ArrValue);
		TIM_SetCompare2(&TIM12_Handler,ArrValue);
	}	
	else if(footnumber == LH_J3)
	{
	//	User_PWM_SetValue(&htim4, TIM_CHANNEL_1,ArrValue);
		TIM_SetCompare1(&TIM15_Handler,ArrValue);
	}	
	else if(footnumber == RF_J1)
	{
	//	User_PWM_SetValue(&htim3, TIM_CHANNEL_3,ArrValue);
		TIM_SetCompare1(&TIM1_Handler,ArrValue);
	}
	else if(footnumber == RF_J2)
	{
	//	User_PWM_SetValue(&htim3, TIM_CHANNEL_2,ArrValue);
		TIM_SetCompare2(&TIM1_Handler,ArrValue);
	}
	else if(footnumber == RF_J3)
	{
	//	User_PWM_SetValue(&htim1, TIM_CHANNEL_2,ArrValue);
		TIM_SetCompare3(&TIM1_Handler,ArrValue);
	}
	else if(footnumber == RH_J1)
	{
//		User_PWM_SetValue(&htim1, TIM_CHANNEL_3,ArrValue);
		TIM_SetCompare1(&TIM3_Handler,ArrValue);
	}
	else if(footnumber == RH_J2)
	{
//		User_PWM_SetValue(&htim1, TIM_CHANNEL_3,ArrValue);
		TIM_SetCompare2(&TIM3_Handler,ArrValue);
	}
	else if(footnumber == RH_J3)
	{
//		User_PWM_SetValue(&htim1, TIM_CHANNEL_3,ArrValue);
		TIM_SetCompare3(&TIM3_Handler,ArrValue);
	}
}