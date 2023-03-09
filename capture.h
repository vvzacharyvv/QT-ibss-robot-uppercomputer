#ifndef _CAPTURE_H
#define _CAPTURE_H
#include "sys.h"


extern u8 operating_flag;
extern u8 pause_time;

extern uint16_t Time8Channel3HighTime;
extern uint16_t Time8Channel4HighTime;

void TIM8_Cap_Init();
//void All_Capture_IT_Start(void);

#endif
