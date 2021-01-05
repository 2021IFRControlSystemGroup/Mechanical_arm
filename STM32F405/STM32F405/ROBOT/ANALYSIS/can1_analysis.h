#ifndef __CAN1_ANALYSIS_H
#define __CAN1_ANALYSIS_H

#include "bsp.h"  

void Receive_Deal_Yun(CanRxMsg *Rx_message);

void Hand_Shake1(uint32_t ID);

void Motor_Eable1(uint32_t ID);

void Motor_Postion1(uint32_t ID,int32_t pos_tar,float speed_tar);

void Motor_Calibrate1(uint32_t ID);

#endif
