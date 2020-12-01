#ifndef __CAN1_ANALYSIS_H
#define __CAN1_ANALYSIS_H

#include "bsp.h"  

void Receive_Deal_Yun(CanRxMsg *Rx_message);

void Hand_Shake1(void);

void Hand_Shake2(void);

void Motor_Eable1(void);

void Motor_Eable2(void);

void Motor_Postion1(int32_t pos);

void Motor_Postion2(int32_t pos);

#endif
