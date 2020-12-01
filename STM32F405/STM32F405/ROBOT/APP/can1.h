#ifndef __CAN1_H
#define __CAN1_H

#include "stm32f4xx.h"

void CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);

void CAN1_RX0_IRQHandler(void);

#endif



