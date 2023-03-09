#ifndef __FORCESENSOR_H__
#define __FORCESENSOR_H__

#include <sys.h>

void GetForce_init(void);
void GetForce(void);
void ShowF1(void);
char* itoa(int num,char* str,int radix);
extern int F1[3];
extern int F2[3];
#endif
