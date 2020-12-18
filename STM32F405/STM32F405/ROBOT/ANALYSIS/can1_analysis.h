#ifndef __CAN1_ANALYSIS_H
#define __CAN1_ANALYSIS_H

#include "bsp.h"  

void Receive_Deal_Yun(CanRxMsg *Rx_message);

void Hand_Shake(uint32_t ID);

void Motor_Eable(uint32_t ID);

void Motor_Postion(uint32_t ID,int32_t pos);

void Motor_Calibrate(uint32_t ID);

#endif
