#ifndef __CAN2_H
#define __CAN2_H

#include "stm32f4xx.h"

u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
 
u8 CAN2_Send_Msg(u8* msg,uint32_t ID);						//发送数据

#endif



