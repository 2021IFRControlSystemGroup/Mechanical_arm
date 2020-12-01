#ifndef __ARM_H
#define __ARM_H

#include "stm32f4xx.h"
#include "can1_analysis.h"
#include "math.h"

void Arm_Init(void);

void Arm_task(void);

void Angle(void);

void Motor_Calibrate(void);

void Arm_Workspace_Judge(void);

#endif


