#ifndef _MOVE_H
#define _MOVE_H
#include "stdint.h"
#include <sys.h>
#define JonintNum 12
#define RF_J1 0
#define LF_J1 1
#define RH_J1 2
#define LH_J1 3

#define RF_J2 4
#define LF_J2 5
#define RH_J2 6
#define LH_J2 7

#define RF_J3 8
#define LF_J3 9
#define RH_J3 10
#define LH_J3 11
#define RF 100
#define RH 200
#define LF 300
#define LH 400

//typedef short int int16_t;
//typedef signed char int8_t; 
void movement(void);
extern float cx[4];
extern float cy[4];
extern float cz[4];
extern const short L1;
extern const short L2;
extern const short L3;
void StartAngleInit(void);                              
void InitRobotPosion(void);                                
void Angle(float,int);
void setcurrentposition (int,float,float,float);
void move(void);
typedef struct Kinematics
{	
	float StartAngle[JonintNum];
	uint32_t StepNum;//Êý¾Ý²½¾à
}KinematicsArm;

extern KinematicsArm KMGecko;
#endif