#ifndef __CAN2_ANALYSIS_H
#define __CAN2_ANALYSIS_H

#include "bsp.h"

void Hand_Shake2(uint32_t ID);

void Motor_Eable2(uint32_t ID);

void Motor_Postion2(uint32_t ID,int32_t pos_tar,float speed_tar);

void Motor_Calibrate2(uint32_t ID);

#endif
