#ifndef __YUN__H
#define __YUN__H

#include "bsp.h"

typedef struct
{
	s32 position;               //电机反馈机械角度
	s32 Position_tar;           //目标速度
	s32 Absolute_offset_pos;    //上电标定（绝对位置）
	s32 Last_position;          //上次位置（机械角度）
	s32 Absolute_position;      //绝对位置
	s16 speed;                  //速度
	s16 Speed_tar;              //目标速度
	s16 torque;                 //机械转矩
	s16 temperature;            //温度
	float moto_output;          //输出值
}YUN_DATA;

void yun(void);
void Yun_PID_Init(void);

#endif
