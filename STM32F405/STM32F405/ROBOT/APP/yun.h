#ifndef __YUN__H
#define __YUN__H

#include "bsp.h"

typedef struct
{
	s32 position;               //���������е�Ƕ�
	s32 Position_tar;           //Ŀ���ٶ�
	s32 Absolute_offset_pos;    //�ϵ�궨������λ�ã�
	s32 Last_position;          //�ϴ�λ�ã���е�Ƕȣ�
	s32 Absolute_position;      //����λ��
	s16 speed;                  //�ٶ�
	s16 Speed_tar;              //Ŀ���ٶ�
	s16 torque;                 //��еת��
	s16 temperature;            //�¶�
	float moto_output;          //���ֵ
}YUN_DATA;

void yun(void);
void Yun_PID_Init(void);

#endif
