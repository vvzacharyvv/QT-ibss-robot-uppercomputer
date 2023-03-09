#include "gait.h"
#include "matrix.h"
#include "pwm.h"
#include "uart.h"
#include "timer.h"
#include "SV.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#define PI 3.1415926
#define onestepH_f 	20		//	20		16
#define onestepH_h 	25		//	30		18
#define StancePullHight 7
#define StancePullHightDiag -6
#define transferForceX 5

#define GAIT_MODE 2
/*
0		air_control_trot();
1		air_control_trot_T4u();
2		air_control_amble();
3		air_control_amble_T4u();
*/
#define TGait  8	 // timeGait
float pressHightBuffer[4]={14, 12, 12, 14};	
float offset[12]= {	
10,15,13,	
10,-15,8,	
30,25,4,	
20,-12,2	
};
/*	PCB-3.4		*/
//10,15,6,	
//10,-15,6,	
//50,25,4,	
//20,-12,-2	

int updateStatus, gaitMode = GAIT_MODE;
float timeGaitUpdate = TGait;

/**
 * @brief Update parameter of gait with Global Variables which extern declare in gait.h, 
 * at the beginning of a new cycle, and the commands come from VOFA+.
 * 
 * @param p the address of struct CreepMotionControl p
 */
void updateGaitParameter(struct_MC *p)
{
	if(updateStatus==1 && p->presentTime <= p->timePeriod)
	{
		updateStatus = 0;
		p->presentTime = 0;

		p->timeGait = timeGaitUpdate;
		switch(gaitMode)
		{
			case 0x00:
				float timeForSwingPhase0[]={ 	8*p->timeGait/16, 	15*p->timeGait/16,	
																				0, 		 		7*p->timeGait/16,		
																				0, 	 	 		7*p->timeGait/16,		
																				8*p->timeGait/16, 	15*p->timeGait/16};
				MatSetVal(&p->timeForSwingPhase, timeForSwingPhase0); 
				break;
			case 0x01:
				float timeForSwingPhase1[]={ 13*p->timeGait/26,	20*p->timeGait/26,	
																						0,	 7*p->timeGait/26,		
																						0,	 7*p->timeGait/26,	
																			13*p->timeGait/26,	20*p->timeGait/26};	
				MatSetVal(&p->timeForSwingPhase, timeForSwingPhase1); 
				break;
			case 0x02:
				float timeForSwingPhase2[]={ 	8*p->timeGait/16, 		11*p->timeGait/16,		
																				0,		 		 					3*p->timeGait/16,		
																				12*p->timeGait/16, 	15*p->timeGait/16,		
																				4*p->timeGait/16, 	7*p->timeGait/16};	
				MatSetVal(&p->timeForSwingPhase, timeForSwingPhase2); 
				break;
			case 0x03:

				break;
			case 0x04:	//	offset of initial position	in 90 degrees
				float offset90[12]={	
							10,-15,0,	
							10,15,0,	
							10,5,0,	
							10,-5,0	
							};
				for(int i=0; i<12; i++)
					offset[i] = offset90[i];
				break;
			case 0x05:	//	offset of initial position	in 180 degrees
				float offset180[12]={	
							10,-15,5,	
							10,15,5,	
							10,5,5,	
							10,-5,5	
							};
				for(int i=0; i<12; i++)
					offset[i] = offset180[i];
				break;
			case 0x06:	
				
				break;


			case 0xFF:	//	offset of initial position
				float offset0[12]={	0	};
				for(int i=0; i<12; i++)
					offset[i] = offset0[i];
				break;
		}
		
		for(int legNum=0; legNum<4; legNum++)	// time for swingphase 
			p->timeForSwing.element[legNum][0] = p->timeForSwingPhase.element[legNum][1] - p->timeForSwingPhase.element[legNum][0]; 
		Mat matOffset;
		MatCreate(&matOffset, 4, 3);
		MatSetVal(&matOffset,offset); 
		MatAdd(&matOffset, &p->ftsPos, &p->ftsPos);
		MatSub(&p->ftsPos, &p->footPosOffset, &p->ftsPos);
		MatCopy(&matOffset, &p->footPosOffset);
		MatDelete(&matOffset);

		for(int i=0; i<4; i++)
			p->pressHight[i] = pressHightBuffer[i];
	}
}

/**
 * @brief Initial of parameters, calculate offset of motors from offset of foot position and set joint position.
 * 
 * @param p 
 */
void CLASSMC_initiation(struct_MC *p)
{
	CLASSMC_defMatrix(p);
	p->times = 0;
	p->width = 60.0;
	p->length = 70.0;
	p->L1 = 45;
	p->L2 = 50;		
	p->L2_f = 50;		//f
	p->L2_h = 60;	//h
	p->L3 = 22;  
	p->timeGait = TGait; 
	p->timePeriod = 0.02;
	p->presentTime= 0;
	#if(GAIT_MODE==0)
	float timeForSwingPhase[]={ 	8*TGait/16, 	15*TGait/16,	
																	0, 		 		7*TGait/16,		
																	0, 	 	 		7*TGait/16,		
																	8*TGait/16, 	15*TGait/16};
	#elif(GAIT_MODE==1)
	float timeForSwingPhase[]={ 13*TGait/26,	20*TGait/26,		//lf		????��??
																			0,	 7*TGait/26,		//rf
																			0,	 7*TGait/26,		//lh
																13*TGait/26,	20*TGait/26};	//rh
	#elif(GAIT_MODE==2)
	//	RF first
//	float timeForSwingPhase[]={ 	8*TGait/16, 		11*TGait/16,		//lf	
//																	0,		 		 	3*TGait/16,			//rf
//																	4*TGait/16, 		7*TGait/16,			//lh
//																	12*TGait/16, 	 	15*TGait/16,};	//rh	
	float timeForSwingPhase[]={ 	8*TGait/16, 		11*TGait/16,		//lf	
	 																0,		 		 	3*TGait/16,			//rf
	 																12*TGait/16, 	 	15*TGait/16,		//lh
	 																4*TGait/16, 		7*TGait/16,};		//rh		
	#elif(GAIT_MODE==3)																
	float timeForSwingPhase[]={ 	6*TGait/12, 	7*TGait/12,		//lf	?????��??
																		0*TGait, 		 	TGait/12,		//rf
																	9*TGait/12,  10*TGait/12,		//lh
																	3*TGait/12, 	4*TGait/12,};	//rh		
	#endif
	MatSetVal(&p->timeForSwingPhase,timeForSwingPhase); 
	float t_shoulderPos[]={p->length/2, p->width/2, 0, p->length/2, -p->width/2, 0, -p->length/2, p->width/2, 0, -p->length/2, -p->width/2, 0}; // X-Y-Z-alpha: LF, RF, LH, RH
	MatSetVal(&p->shoulderPos,t_shoulderPos);

	float tempFtsPos[]={		
	p->L2_f, p->L1, -p->L3, 
	p->L2_f, -p->L1, -p->L3,
	-p->L2_h, p->L1, -p->L3,
	-p->L2_h, -p->L1, -p->L3};
	MatSetVal(&p->ftsPos,tempFtsPos);
	MatSetVal(&p->stancePhaseStartPos,tempFtsPos);
	MatSetVal(&p->stancePhaseEndPos,tempFtsPos);

	for(int i=0; i<4; i++)
		p->pressHight[i] = pressHightBuffer[i];

	MatAdd(&p->ftsPos, &p->shoulderPos, &p->footPos);
	for(int legNum=0; legNum<4; legNum++)	// time for swingphase 
		p->timeForSwing.element[legNum][0] = p->timeForSwingPhase.element[legNum][1] - p->timeForSwingPhase.element[legNum][0]; 

	/*	calculate initial offset of motors and set position	*/
	MatZeros(&p->motorOffset);
	MatSetVal(&p->footPosOffset,offset); 
	MatAdd(&p->footPosOffset, &p->ftsPos, &p->ftsPos);
	CLASSMC_inverseKinematics(p);
	MatCopy(&p->motorPos, &p->motorOffset);
	/*	Recover ftsPos	*/
	MatCopy(&p->stancePhaseStartPos, &p->ftsPos);

	CLASSMC_setJointPosition(p);	
}
void CLASSMC_defMatrix(struct_MC *p)
{
	MatCreate(&p->timeForStancePhase, 4, 2);// startTime, endTime: LF, RF, LH, RH
	MatCreate(&p->timeForSwingPhase, 4, 2);// startTime, endTime: LF, RF, LH, RH
	MatCreate(&p->timeForSwing, 4, 1);  
	MatCreate(&p->targetCoMVelocity, 3, 1);  // X, Y , alpha c in world cordinate 3x1
	MatCreate(&p->comPos, 3, 1);  
	MatCreate(&p->shoulderPos,4,3);  // X-Y: LF, RF, LH, RH
	MatCreate(&p->footPos,4,3);
	MatCreate(&p->footPosOffset, 4, 3);
	MatCreate(&p->motorPos,4,3);
	MatCreate(&p->motorOffset,4,3);
	MatCreate(&p->ftsPos,4,3);
	MatCreate(&p->stancePhaseStartPos,4,3);
	MatCreate(&p->stancePhaseEndPos,4,3);
	MatCreate(&p->targetPos, 4, 3);
	MatCreate(&p->swingPhaseVelocity, 4, 3);
}
void CLASSMC_freeMatrix(struct_MC *p)
{
	MatDelete(&p->timeForStancePhase);// startTime, endTime: LF, RF, LH, RH
	MatDelete(&p->timeForSwingPhase);// startTime, endTime: LF, RF, LH, RH
	MatDelete(&p->timeForSwing);
	MatDelete(&p->targetCoMVelocity);  // X, Y , alpha c in world cordinate
	MatDelete(&p->comPos); 
	MatDelete(&p->shoulderPos);  // X-Y: LF, RF, LH, RH
	MatDelete(&p->footPos);
	MatDelete(&p->footPosOffset);
	MatDelete(&p->motorPos);
	MatDelete(&p->motorOffset);
	MatDelete(&p->ftsPos);
	MatDelete(&p->stancePhaseStartPos);
	MatDelete(&p->stancePhaseEndPos);
	MatDelete(&p->targetPos);
	MatDelete(&p->swingPhaseVelocity);
}
void CLASSMC_setCoMVel(struct_MC *p, float* val)
{
	MatSetVal(&p->targetCoMVelocity,val);
}

void CLASSMC_nextStep(struct_MC *p)
{
	if(p->times>=0)   
	{
		for(int i=0; i<3; i++)
		{
			p->comPos.element[i][0] = p->comPos.element[i][0] + p->timePeriod * p->targetCoMVelocity.element[i][0];
		}
		for(int legNum=0; legNum<4; legNum++)  // run all 4 legs
		{   
			// for swingphase 
			if(p->presentTime > p->timeForSwingPhase.element[legNum][0] + p->timePeriod/2 && p->presentTime < p->timeForSwingPhase.element[legNum][1] + p->timePeriod/2) 
			{  
				p->swingFlag[legNum] = 1;
				p->pressFlag = 1;
				for(int i=0; i<3; i++){
					p->swingPhaseVelocity.element[legNum][i] = (p->stancePhaseEndPos.element[legNum][i] - p->stancePhaseStartPos.element[legNum][i]) / (p->timeForSwing.element[legNum][0]);
				}
	
				float onestepW = fabs(p->stancePhaseEndPos.element[legNum][1] - p->stancePhaseStartPos.element[legNum][1]);
				float onestepL = fabs(p->stancePhaseEndPos.element[legNum][0] - p->stancePhaseStartPos.element[legNum][0]);   
  		
				if(p->presentTime - p->timeForSwingPhase.element[legNum][0] >  p->timePeriod/2 &&  p->presentTime - p->timeForSwingPhase.element[legNum][0] <  p->timeForSwing.element[legNum][0]*0.4 + p->timePeriod/2)
				{
					p->ftsPos.element[legNum][0] -= p->swingPhaseVelocity.element[legNum][0] / 0.6 * p->timePeriod;	// compress distance(Velocity) in swing phase to 0.6*Tsw 
					p->ftsPos.element[legNum][1] -= p->swingPhaseVelocity.element[legNum][1] / 0.6 * p->timePeriod;

					if(legNum==0||legNum==1)  //onestepW	legNum==0||legNum==2
					{             
						if(onestepL !=0 )       
							//p->ftsPos.element[legNum][2] = -9/4 * onestepH/(onestepW*onestepW)*(p->ftsPos.element[legNum][1]-p->L1+onestepW/3)*(p->ftsPos.element[legNum][1]-p->L1+onestepW/3)+onestepH -p->L3;
							p->ftsPos.element[legNum][2] = -9/4.0 * onestepH_f/(onestepL*onestepL)*(p->ftsPos.element[legNum][0] - p->stancePhaseEndPos.element[legNum][0] - onestepL * 2/3)*(p->ftsPos.element[legNum][0] - p->stancePhaseEndPos.element[legNum][0] - onestepL * 2/3) + onestepH_f + p->stancePhaseEndPos.element[legNum][2];	
						else
							p->ftsPos.element[legNum][2] += onestepH_f / p->timeForSwing.element[legNum][0] / 0.4 * p->timePeriod;
					} 
					if(legNum==2||legNum==3)		//onestepW	legNum==1||legNum==3
					{
						if(onestepL !=0 )
							//p->ftsPos.element[legNum][2] = -9/4 * onestepH/(onestepW*onestepW)*(p->ftsPos.element[legNum][1]+p->L1+onestepW/3)*(p->ftsPos.element[legNum][1]+p->L1+onestepW/3)+onestepH -p->L3; 
							p->ftsPos.element[legNum][2] = -9/4.0 * onestepH_h/(onestepL*onestepL)*(p->ftsPos.element[legNum][0] - p->stancePhaseEndPos.element[legNum][0] - onestepL * 2/3)*(p->ftsPos.element[legNum][0] - p->stancePhaseEndPos.element[legNum][0] - onestepL * 2/3) + onestepH_h + p->stancePhaseEndPos.element[legNum][2]; 			//L2_h		legNum==2||legNum==3								
						else
							p->ftsPos.element[legNum][2] += onestepH_f / p->timeForSwing.element[legNum][0] / 0.4 * p->timePeriod;
					} 
				}
				else if(p->presentTime - p->timeForSwingPhase.element[legNum][0] > p->timeForSwing.element[legNum][0]*0.4 + p->timePeriod/2 && p->presentTime-p->timeForSwingPhase.element[legNum][0] < p->timeForSwing.element[legNum][0]*0.6 + p->timePeriod/2)
				{																																																																											//											p->timeForSwing.element[legNum][0]*0.8	p->timeForSwingPhase.element[legNum][1]
					p->ftsPos.element[legNum][0] -= p->swingPhaseVelocity.element[legNum][0] / 0.6 * p->timePeriod;
					p->ftsPos.element[legNum][1] -= p->swingPhaseVelocity.element[legNum][1] / 0.6 * p->timePeriod;
					if(legNum==0||legNum==1)
						p->ftsPos.element[legNum][2] -= onestepH_f / p->timeForSwing.element[legNum][0] /(0.6-0.4) * p->timePeriod;
					if(legNum==2||legNum==3)
						p->ftsPos.element[legNum][2] -= onestepH_h / p->timeForSwing.element[legNum][0] /(0.6-0.4) * p->timePeriod;	
				}
				else if(p->presentTime - p->timeForSwingPhase.element[legNum][0] > p->timeForSwing.element[legNum][0]*0.6 + p->timePeriod/2 && p->presentTime-p->timeForSwingPhase.element[legNum][0] < p->timeForSwing.element[legNum][0]*0.68 + p->timePeriod/2) 
				{
					p->pressFlag = 2;
					p->ftsPos.element[legNum][0] += p->swingPhaseVelocity.element[legNum][0] *2* p->timePeriod;	//	make the foot stationary relative to the ground on x, y
					p->ftsPos.element[legNum][1] += p->swingPhaseVelocity.element[legNum][1] *2* p->timePeriod;
					p->ftsPos.element[legNum][2] -= p->pressHight[legNum] / p->timeForSwing.element[legNum][0] / 0.08 * p->timePeriod;//	down			
				}
				else if(p->presentTime - p->timeForSwingPhase.element[legNum][0] > p->timeForSwing.element[legNum][0]*0.68 + p->timePeriod/2 && p->presentTime - p->timeForSwingPhase.element[legNum][0] < p->timeForSwing.element[legNum][0]*0.92 + p->timePeriod/2)
				{
					p->pressFlag = 1;
				}
				else if(p->presentTime - p->timeForSwingPhase.element[legNum][0] > p->timeForSwing.element[legNum][0]*0.92 + p->timePeriod/2 && p->presentTime < p->timeForSwingPhase.element[legNum][1] + p->timePeriod/2)
				{
					p->pressFlag = 3;
					p->ftsPos.element[legNum][0] -= p->swingPhaseVelocity.element[legNum][0] *2* p->timePeriod;
					p->ftsPos.element[legNum][1] -= p->swingPhaseVelocity.element[legNum][1] *2* p->timePeriod;
					p->ftsPos.element[legNum][2] += p->pressHight[legNum] / p->timeForSwing.element[legNum][0] / 0.08 * p->timePeriod;//					up				
				}		
				if( fabs(p->presentTime - p->timeForSwingPhase.element[legNum][1]) < p->timePeriod)	//	swing phase end
				{
					p->pressFlag = 0;
					p->swingFlag[legNum] = 0;  
				}

				p->footPos.element[legNum][0] = p->ftsPos.element[legNum][0] + p->shoulderPos.element[legNum][0] + p->comPos.element[0][0];
				p->footPos.element[legNum][1] = p->ftsPos.element[legNum][1] + p->shoulderPos.element[legNum][1] + p->comPos.element[1][0];			
				p->footPos.element[legNum][2] = p->ftsPos.element[legNum][2];
			} 
			// for stancephase
			else if(p->presentTime  < p->timeForSwingPhase.element[legNum][0] + p->timePeriod/2 || p->presentTime > p->timeForSwingPhase.element[legNum][1] + p->timePeriod/2)
			{        
				// p->swingFlag[legNum] = 0;            
												
				Mat trans;
				MatCreate(&trans, 4, 4);
				float t_trans[]={cos(p->comPos.element[2][0]), -sin(p->comPos.element[2][0]), 0, p->comPos.element[0][0],
						sin(p->comPos.element[2][0]), cos(p->comPos.element[2][0]), 0, p->comPos.element[1][0],
						0, 0, 1, 0,
						0, 0, 0, 1};
				
				MatSetVal(&trans,t_trans);	
												
				Mat oneShoulderPos_4x1;
				MatCreate(&oneShoulderPos_4x1, 4, 1);
				float t_oneShoulderPos_4x1[]={p->shoulderPos.element[legNum][0], p->shoulderPos.element[legNum][1], 0, 1};
				MatSetVal(&oneShoulderPos_4x1,t_oneShoulderPos_4x1);
				Mat shoulderPos_4x1;	// shoulder to world
				MatCreate(&shoulderPos_4x1, 4, 1);
				MatMul(&trans, &oneShoulderPos_4x1, &shoulderPos_4x1); 
				float Trans[4]={0};
				for(int i=0;i<4; i++)
				{
					//for(int j=0;j<2; j++)
					//{
						Trans[i]=shoulderPos_4x1.element[i][0];
					//}
				}				

			// if(p->times !=0)		//	pull back the distance that press in the end of swing phase 
			// {
			// 	if(p->presentTime > p->timeForSwingPhase.element[legNum][1] + (p->timeGait - p->timeForSwing.element[legNum][0]) * 0.1 && p->presentTime  < p->timeForSwingPhase.element[legNum][1] + (p->timeGait - p->timeForSwing.element[legNum][0]) * 0.2)
			// 		p->ftsPos.element[legNum][2] += p->pressHight[legNum] / (0.2-0.1) / (p->timeGait - p->timeForSwing.element[legNum][0]) * p->timePeriod; // 7.5 / 0.2 / (p->timeGait - p->timeForSwing.element[legNum][0]) 
			// 	else if(p->presentTime > p->timeForSwingPhase.element[legNum][0] - (p->timeGait - p->timeForSwing.element[legNum][0]) * 0.9 && p->presentTime  < p->timeForSwingPhase.element[legNum][0] - (p->timeGait - p->timeForSwing.element[legNum][0]) * 0.8)
			// 		p->ftsPos.element[legNum][2] += p->pressHight[legNum] / (0.9-0.8) / (p->timeGait - p->timeForSwing.element[legNum][0]) * p->timePeriod;
			// }

				/*	make body close to the 180 floor	*/
				// float stancePull[4]={StancePullHight,StancePullHight,StancePullHight,StancePullHight};
				// int i=0;
				// for(i=0;i<4;i++)
				// 	if(p->swingFlag[i]==1)
				// 		break;
				// switch (i)
				// {
				// case 0:
				// 	stancePull[3] = StancePullHightDiag;
				// 	break;
				// case 1:
				// 	stancePull[2] = StancePullHightDiag;
				// 	break;
				// case 2:
				// 	stancePull[1] = StancePullHightDiag;
				// 	break;
				// case 3:
				// 	stancePull[0] = StancePullHightDiag;
				// 	break;
				// }
				// if( p->pressFlag != 0)	//p->times !=0 &&
				// {
				// 	if( (p->presentTime - p->timeForSwingPhase.element[i][0]) < 0.08 * p->timeForSwing.element[legNum][0] )	
				// 		p->ftsPos.element[legNum][2] += stancePull[legNum] / p->timeForSwing.element[legNum][0] / 0.08  * p->timePeriod; 
				// 	// if(p->pressFlag == 2)	
				// 	// 	p->ftsPos.element[legNum][2] += stancePull[legNum] / p->timeForSwing.element[legNum][0] / 0.08  * p->timePeriod; 
				// 	if(p->pressFlag == 3)	
				// 		p->ftsPos.element[legNum][2] -= stancePull[legNum] / p->timeForSwing.element[legNum][0] / 0.08  * p->timePeriod; 
				// }

				/* transfer force	for trot  */
//				if(p->presentTime  > p->timeGait * 7/16 && p->presentTime  < p->timeGait * 8/16)	
//				{
//					if(legNum==0)
//						p->footPos.element[0][0] +=  transferForceX / p->timeGait * p->timePeriod *16;
//					else if(legNum==3)
//						p->footPos.element[3][0] +=  transferForceX / p->timeGait * p->timePeriod *16;
//				}
//				if(p->presentTime  > p->timeGait * 15/16 && p->presentTime  < p->timeGait)	
//				{
//					if(legNum==1)
//						p->footPos.element[1][0] +=  transferForceX / p->timeGait * p->timePeriod *16;
//					else if(legNum==2)
//						p->footPos.element[2][0] +=  transferForceX / p->timeGait * p->timePeriod *16;
//				}

				/* transfer force	for amble  */
				// if(p->presentTime  > p->timeGait * 3/16 && p->presentTime  < p->timeGait * 4/16 && legNum==3)
				// 	p->footPos.element[3][0] +=  transferForceX / p->timeGait * p->timePeriod *16;
				// if(p->presentTime  > p->timeGait * 7/16 && p->presentTime  < p->timeGait * 8/16 && legNum==0)
				// 	p->footPos.element[0][0] +=  transferForceX / p->timeGait * p->timePeriod *16;
				// if(p->presentTime  > p->timeGait * 11/16 && p->presentTime  < p->timeGait * 12/16 && legNum==2)
				// 	p->footPos.element[2][0] +=  transferForceX / p->timeGait * p->timePeriod *16;
				// if(p->presentTime  > p->timeGait * 15/16 && p->presentTime  < p->timeGait  && legNum==1)
				// 	p->footPos.element[1][0] +=  transferForceX / p->timeGait * p->timePeriod *16;

				/*	update ftsPos	*/
				p->ftsPos.element[legNum][0] = p->footPos.element[legNum][0] - shoulderPos_4x1.element[0][0];  // X
				p->ftsPos.element[legNum][1] = p->footPos.element[legNum][1] - shoulderPos_4x1.element[1][0];  // Y

				/*	record the foot position of stance phase on start and end		*/
				if(fabs(p->presentTime - p->timeForSwingPhase.element[legNum][1]) < p->timePeriod) 
				{
					for(int i=0; i<3; i++)
						p->stancePhaseStartPos.element[legNum][i] = p->ftsPos.element[legNum][i];
				}
				if(p->times == 0 && p->presentTime < p->timePeriod/2) 
				{
					for(int i=0; i<3; i++)
						p->stancePhaseStartPos.element[legNum][i] = p->ftsPos.element[legNum][i];              
				}
				if(fabs(p->presentTime - p->timeForSwingPhase.element[legNum][0]) < p->timePeriod)  
				{
					for(int i=0; i<3; i++){
						p->stancePhaseEndPos.element[legNum][i] = p->ftsPos.element[legNum][i];} 				
				}

				MatDelete(&oneShoulderPos_4x1);
				MatDelete(&shoulderPos_4x1);
				MatDelete(&trans);
			} 
		} 
	}
	
	p->presentTime += p->timePeriod;
	if(fabs(p->presentTime - p->timeGait) < 1e-3)  // check if present time has reach the gait period                                                               
	{                                                    
		p->presentTime = 0.0;
		p->times++;
		for(int i=0; i<3; i++)	//	remove the accumulated error in z
			p->ftsPos.element[i][2] = -p->L3;
	}
}
void fts(struct_MC *p)
{
	for(int i = 0; i<4; i++)
	{
		for(int j = 0; j<3; j++)
		{
			p->ftsPosition[i*3+j] = p->ftsPos.element[i][j];
		}
	}
}
/**
 * @brief Calculate motorPos with ftsPos and motorOffset
 * 
 * @param p 
 */
void CLASSMC_inverseKinematics(struct_MC *p) //ftsPos -> motorPos
{   
	for(uint8_t legNum=0; legNum<4; legNum++)  // LF RF LH RH
	{
		float factor_y, factor_x, factor_xc, factor_yc, factor_zc;  // factor for x/y; factor for whole formula
		if(legNum==0)
		{
			factor_xc=-1;
			factor_yc=1;
			factor_zc=1;
			factor_x=1;
			factor_y=1;
			p->L2 = p->L2_f;
		}
		if(legNum==1)
		{
			factor_xc=1;
			factor_yc=-1;
			factor_zc=-1;
			factor_x=1;
			factor_y=-1;
			p->L2 = p->L2_f;
		}
		if(legNum==2)
		{
			factor_xc=-1;
			factor_yc=-1;
			factor_zc=-1;
			factor_x=-1;
			factor_y=1;
			p->L2 = p->L2_h;
		}
		if(legNum==3)
		{
			factor_xc=1;
			factor_yc=1;
			factor_zc=1;
			factor_x=-1;
			factor_y=-1;
			p->L2 = p->L2_h;
		}
		p->motorPos.element[legNum][1] = p->motorOffset.element[legNum][1] - factor_xc * (asin(p->L3 / sqrtf( p->ftsPos.element[legNum][2]* p->ftsPos.element[legNum][2] + p->ftsPos.element[legNum][1]* p->ftsPos.element[legNum][1] )) + atan2( p->ftsPos.element[legNum][2],factor_y * p->ftsPos.element[legNum][1]) );     
		p->motorPos.element[legNum][0] = p->motorOffset.element[legNum][0] - factor_yc * (asin(( p->ftsPos.element[legNum][1] * p->ftsPos.element[legNum][1] + p->ftsPos.element[legNum][0] *  p->ftsPos.element[legNum][0] + p->ftsPos.element[legNum][2] * p->ftsPos.element[legNum][2] + p->L1 * p->L1 - p->L2 * p->L2 - p->L3 * p->L3) / ( 2 * p->L1 * sqrtf ( p->ftsPos.element[legNum][1] * p->ftsPos.element[legNum][1] + p->ftsPos.element[legNum][0] * p->ftsPos.element[legNum][0] + p->ftsPos.element[legNum][2] * p->ftsPos.element[legNum][2] - p->L3 * p->L3)))
						- atan2(sqrtf( p->ftsPos.element[legNum][1] * p->ftsPos.element[legNum][1] + p->ftsPos.element[legNum][2] * p->ftsPos.element[legNum][2] - p->L3 * p->L3) , factor_x * p->ftsPos.element[legNum][0]));
		p->motorPos.element[legNum][2] = p->motorOffset.element[legNum][2] - factor_zc * asin((p->L1 * p->L1 + p->L2 * p->L2 + p->L3 * p->L3 - p->ftsPos.element[legNum][1] * p->ftsPos.element[legNum][1] - p->ftsPos.element[legNum][0] * p->ftsPos.element[legNum][0] - p->ftsPos.element[legNum][2] * p->ftsPos.element[legNum][2]) / (2 * p->L1 * p->L2));
	}
}
void CLASSMC_setJointPosition(struct_MC *p)
{
	float theta[4][2]={0};

	theta[0][0]=(p->motorPos.element[0][0]+p->motorPos.element[0][1]);
	theta[0][1]=(p->motorPos.element[0][0]-p->motorPos.element[0][1]);
	theta[1][0]=(p->motorPos.element[1][0]+p->motorPos.element[1][1]);
	theta[1][1]=(p->motorPos.element[1][0]-p->motorPos.element[1][1]);
	theta[2][0]=(p->motorPos.element[2][0]-p->motorPos.element[2][1]);
	theta[2][1]=(p->motorPos.element[2][0]+p->motorPos.element[2][1]);
	theta[3][0]=(p->motorPos.element[3][0]-p->motorPos.element[3][1]);
	theta[3][1]=(p->motorPos.element[3][0]+p->motorPos.element[3][1]);
	int16_t ArrValue[12] = {0};
	int16_t angle[12] = {0};
	for(int i = 0; i<4; i++)
	{
		for(int j = 0; j<2; j++)
		{
			p->jointPos[i*3+j] = theta[i][j];       
		}
	}
	
	for(int i = 0; i<4; i++)
	{
		p->jointPos[i*3+2] = p->motorPos.element[i][2];
	}
	
	for(int i=0; i<12; i++)
	{
		angle[i] = -p->jointPos[i]*180/PI ;
	}	
	angle[6]=angle[6]+5;
	angle[7]=angle[7]-5;
	angle[9]=angle[9]-7;
	angle[10]=angle[10]+7;
	for(int i=0; i<12; i++)
	{
		ArrValue[i] = (int32_t) (angle[i] * 5.56 + 750);
	}
	TIM_SetCompare1(&TIM4_Handler,ArrValue[2]);	//LF
	TIM_SetCompare2(&TIM4_Handler,ArrValue[1]);
	TIM_SetCompare3(&TIM4_Handler,ArrValue[0]);
	/*	PCB_3.3	*/
	// TIM_SetCompare1(&TIM12_Handler,ArrValue[8]);	//LH
	// TIM_SetCompare2(&TIM12_Handler,ArrValue[7]);
	// TIM_SetCompare1(&TIM15_Handler,ArrValue[6]);
	// TIM_SetCompare1(&TIM1_Handler,ArrValue[5]);	//RF
	// TIM_SetCompare2(&TIM1_Handler,ArrValue[4]);
	// TIM_SetCompare3(&TIM1_Handler,ArrValue[3]);

	/*	PCB_3.4	*/
	TIM_SetCompare1(&TIM12_Handler,ArrValue[5]);	//RF
	TIM_SetCompare2(&TIM12_Handler,ArrValue[4]);
	TIM_SetCompare1(&TIM15_Handler,ArrValue[3]);
	TIM_SetCompare1(&TIM1_Handler,ArrValue[8]);	//LH
	TIM_SetCompare2(&TIM1_Handler,ArrValue[7]);
	TIM_SetCompare3(&TIM1_Handler,ArrValue[6]);

	TIM_SetCompare1(&TIM3_Handler,ArrValue[11]);	//RH
	TIM_SetCompare2(&TIM3_Handler,ArrValue[10]);
	TIM_SetCompare3(&TIM3_Handler,ArrValue[9]);
}
