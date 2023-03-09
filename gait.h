#ifndef _MC
#define _MC
#include <sys.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#ifndef _ENUM_BOOL
#define _ENUM_BOOL
typedef enum bool {true = 1, false = 0}bool;
#endif 
typedef struct CreepMotionControl struct_MC;
extern int updateStatus, gaitMode;
extern float timeGaitUpdate, pressHightBuffer[4];
struct CreepMotionControl
{
	float timeGait;  // The time of the whole cycle
	float timePeriod;	// The time of one step
  float presentTime;
	float times;
	float roll;
	float width;
	float length;
	float L1, L2, L3, L2_f, L2_h;  // The length of L
	int pressFlag;	//	0-stance, 1-swing, 2-attach1, 3-attach2
	int swingFlag[4];	// set 1 means the leg is in swing phase
	float pressHight[4];	//	the pressure height in the end of swing phase

	Mat comPos;  // position of center of mass
//	Mat comPos_bymotor;
	Mat shoulderPos;  // LF RF LH RH X, Y  shoulder to COM 
//	Mat shoulderPos_bymotor;
	Mat footPos;  // LF RF LH RH X, Y, Z
	Mat ftsPos;  // LF RF LH RH X, Y, Z foot_to_shoulder
	Mat footPosOffset;
//	Mat ftsPos_bymotor;
	Mat targetCoMVelocity;  // X, Y , alpha c in world cordinate
	Mat d_targetCoMVelocity;
	//Matrix<float, 4, 3> shoulderPos_for_real;  // real shoulder position by motor feedback
	//Vector<float, 4> com_position;  // coordinate of center of mass
	//Vector<float, 4> com_position_for_real;
	Mat targetPos;
	Mat motorPos;  // LF RF LH RH 1,2,3 motor position command
	Mat motorOffset;
//	Mat motorPosFdb;  // LF RF LH RH 1,2,3 motor position command by motor feedback
	Mat timeForStancePhase;
	Mat timeForSwingPhase;	// the start and end time for swing phase
	Mat timeForSwing;	// the duration of swing phase
	Mat swingPhaseVelocity;
//	Mat targetCoMPosition;  // X, Y , alpha in world cordinate
	Mat stanceFlag;  // True, False: LF, RF, LH, RH


	Mat stancePhaseStartPos;
	Mat stancePhaseEndPos;

  float ftsPosition[12];
	float footposition[12];
	float jointPos[12];
	float shoulderpos[12];

};
void CLASSMC_defMatrix(struct_MC *p);
void CLASSMC_freeMatrix(struct_MC *p);
void CLASSMC_setInitPos(struct_MC *p);
void CLASSMC_setCoMVel(struct_MC *p, float* val);
void CLASSMC_nextStep(struct_MC *p);
void CLASSMC_inverseKinematics(struct_MC *p);
void CLASSMC_forwardKinematics(struct_MC *p);
void CLASSMC_setJointPosition(struct_MC *p);
void CLASSMC_initiation(struct_MC *p);
void CLASSMC_turn(struct_MC *p);
void fts(struct_MC *p);
void updateGaitParameter(struct_MC *p);
//	char positionName[12][22] = {"LF0 PositionSensor", "LF1 PositionSensor", "LF2 PositionSensor", "RF0 PositionSensor", "RF1 PositionSensor", "RF2 PositionSensor", "LH0 PositionSensor", "LH1 PositionSensor", "LH2 PositionSensor", "RH0 PositionSensor", "RH1 PositionSensor", "RH2 PositionSensor"};
//	char motorName[12][22] = {"LF0 RotationalMotor", "LF1 RotationalMotor", "LF2 RotationalMotor", "RF0 RotationalMotor", "RF1 RotationalMotor", "RF2 RotationalMotor", "LH0 RotationalMotor", "LH1 RotationalMotor", "LH2 RotationalMotor", "RH0 RotationalMotor", "RH1 RotationalMotor", "RH2 RotationalMotor"};
//	char touchsensorName[4][16] = {"LF_touch_sensor", "RF_touch_sensor", "LH_touch_sensor", "RH_touch_sensor"};
//        PositionSensor *Ps[12];
//        Motor *Tor[12];
//        TouchSensor *Ts[4];
//        InertialUnit *imu;
//        GPS *gps;
        
//        Vector<float, 3> imu_num;        //VMC
#endif