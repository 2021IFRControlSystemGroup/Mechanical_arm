#ifndef __PID_H
#define __PID_H
#include "sys.h"

typedef struct 
{
	float Kp;                     //��������
  float Ki;                     //���ֳ���
  float Kd;                     //΢�ֳ���
	float Target_value;           //Ŀǰֵ
	float Current_value;          //��ǰֵ
	float Err;                    //���
	float Last_Err;               //��һ�����
	float integral;               //����ֵ
	float dErr;                   //΢��ֵ
	float output;                 //PID���
	float output_max;             //������ֵ
	float output_min;             //�����Сֵ
	float max_integral;
}PID_TypeDef;

void PID_Init(PID_TypeDef*pid,float Kp,float Ki,float Kd,float max_integral,float output_max,float output_min);
s16 PID_operation(PID_TypeDef*pid,float Target_value,float Current_value);

#endif
