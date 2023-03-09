#include "ForceControl.h"
#include "ForceSensor.h"
#include "string.h"
#include <math.h>
#include "can.h"
#include "uart.h"
#include "pwm.h"
#include "gait.h"
#include <stdlib.h>

//typedef struct Kinematics
//{	float StartAngle[JonintNum];
//	uint32_t StepNum;//数据步距
//}KinematicsArm;
float timestep = 0.02;

int Xd_f[3] ={0, 0, 0};
int dXd_f[3] ={0, 0, 0};
int d2Xd_f[3] ={0, 0, 0};

int Xd[3] ={0, 0, 0};
int dXd[3] ={0, 0, 0};
int d2Xd[3] ={0, 0, 0};

int Xc[3] = {0, 0, 0};
int dXc[3] = {0, 0, 0};
int d2Xc[3] = {0, 0, 0};

int Xc_f[3] = {0, 0, 0};
int dXc_f[3] = {0, 0, 0};
int d2Xc_f[3] = {0, 0, 0};

int width = 0;//x zuoyou
int length = 0;//y qianhou
int height = 20;//z gaodu
int depth =20;//-z dixia

float M=1;
float B=1;
float K=0.01;

float F1d[3] = {0.0, 0.0, 0.5};
int T1 = 50;
int n = 0;
float px = 0;
float pz = 0;
float py = 0;

int tt = 0;
void ForceContact()
{
		float a[4][3] = {0};
		if(n<=T1)
		{	
			int step = T1;
			int t=n;
			float h = (float) height / step * n;
			
			px = L1;
			py = L2;
			pz = -L3 + h;
			
			a[0][0] = asin(-L3 / sqrt(px * px + pz * pz) ) - atan2(pz,px);
			a[0][1] = asin((px * px + py * py + pz * pz + L1 * L1 - L2 * L2 - L3 * L3) /
										( 2 * L1 * sqrt (px * px +  py * py + pz * pz - L3 * L3)) ) - 
											atan2(sqrt(px * px + pz * pz - L3 * L3) , py);
			a[0][2] = asin((L1 * L1 + L2 * L2 + L3 * L3 - px * px - py * py - pz * pz) / (2 * L1 * L2));
			
			a[0][0]=(int) (a[0][0]/3.1415926*180*100);//rf alpha 角，最内侧
			a[0][1]=(int) (a[0][1]/3.1415926*180*100);
			a[0][2]=(int) (a[0][2]/3.1415926*180*100);

			a[0][0]= -(float) (a[0][0]/100);
			a[0][1]= -(float) (a[0][1]/100);
			a[0][2]=  (float) (a[0][2]/100);
			
			Angle(a[0][0]+KMGecko.StartAngle[RF_J3],RF_J3);
			Angle(a[0][1]+KMGecko.StartAngle[RF_J2],RF_J2);
			Angle(a[0][2]+KMGecko.StartAngle[RF_J1],RF_J1);
			n += 1;
		}
		else if (n>T1)
		{		
				int tt=n-T1;
				if((F1[2]<(F1d[2]*1000 -50)))
				{
					px = L1;
					py = L2;
					pz = -L3 + height -0.1*tt;
					n += 1;
				}

				Xd[0] = px;Xd[1]=py;Xd[2]=pz;
				dXd[0] =  (Xd[0] - Xd_f[0]) / timestep;dXd[1] =  (Xd[1] - Xd_f[1]) / timestep;dXd[2] =  (Xd[2] - Xd_f[2]) / timestep;
				d2Xd[0] =  (dXd[0] - dXd_f[0]) / timestep;d2Xd[1] =  (dXd[1] - dXd_f[1]) / timestep;d2Xd[2] =  (dXd[2] - dXd_f[2]) / timestep;
				
				//F1[2] = F1[2]/1000;
				d2Xc[2] = d2Xd[2] - ((F1d[2] - (F1[2]/1000)) - B*(dXd[2] - dXc_f[2]) - K*(Xd[2] - Xc_f[2]))/M;
				dXc[2] = dXc_f[2] + d2Xc[2] * timestep;
				Xc[2] = Xc_f[2] + dXc[2] * timestep;
				
				Xc[0] = Xd[0];
				Xc[1] = Xd[1];
				
				a[0][0] = asin(-L3 / sqrt(Xc[0] * Xc[0] + Xc[2] * Xc[2]) ) - atan2(Xc[2],Xc[0]);
				a[0][1] = asin((Xc[0] * Xc[0] + Xc[1] * Xc[1] + Xc[2] * Xc[2] + L1 * L1 - L2 * L2 - L3 * L3) / ( 2 * L1 * sqrt (Xc[0] * Xc[0] +  Xc[1] * Xc[1] + Xc[2] * Xc[2] - L3 * L3)) )
										 - atan2(sqrt(Xc[0] * Xc[0] + Xc[2] * Xc[2] - L3 * L3) , Xc[1]);
				a[0][2] = asin((L1 * L1 + L2 * L2 + L3 * L3 - Xc[0] * Xc[0] - Xc[1] * Xc[1] - Xc[2] * Xc[2]) / (2 * L1 * L2));
				
				for(int i=0;i<3;i++)
				{
					Xd_f[i] = Xd[i];
					dXd_f[i] = dXd[i];
					d2Xd_f[i] = d2Xd[i];
					
					Xc_f[i] = Xc[i];
					dXc_f[i] = dXc[i];
					d2Xc_f[i] = d2Xc[i];
				}
				
				a[0][0]=(int) (a[0][0]/3.1415926*180*100);//rf alpha 角，最内侧
				a[0][1]=(int) (a[0][1]/3.1415926*180*100);
				a[0][2]=(int) (a[0][2]/3.1415926*180*100);

				a[0][0]= -(float) (a[0][0]/100);
				a[0][1]= -(float) (a[0][1]/100);
				a[0][2]=  (float) (a[0][2]/100);
				
				Angle(a[0][0]+KMGecko.StartAngle[RF_J3],RF_J3);
				Angle(a[0][1]+KMGecko.StartAngle[RF_J2],RF_J2);
				Angle(a[0][2]+KMGecko.StartAngle[RF_J1],RF_J1);
			  
		}
			
}	