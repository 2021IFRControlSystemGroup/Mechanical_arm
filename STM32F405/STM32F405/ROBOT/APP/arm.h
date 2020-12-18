#ifndef __ARM_H
#define __ARM_H

#include "stm32f4xx.h"
#include "can1_analysis.h"
#include "math.h"
#include "beep.h" 

#define ID1_HANDSHAKE 0x0008019E
#define ID2_HANDSHAKE 0x0010019E
#define ID3_HANDSHAKE 0x0018019E

#define ID1_ENABLE    0x00080181
#define ID2_ENABLE    0x00100181
#define ID3_ENABLE    0x00180181

#define ID1_POS    0x00080186
#define ID2_POS    0x00100186
#define ID3_POS    0x00180186

#define ID1_ZERO    0x00080189
#define ID2_ZERO    0x00100189
#define ID3_ZERO    0x00180189

void Arm_Init(void);

void Arm_task(void);

void Angle(void);

int Arm_Workspace_Judge(float tarX,float tarY);

#endif


