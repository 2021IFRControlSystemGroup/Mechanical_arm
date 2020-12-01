#ifndef __PID_H
#define __PID_H
#include "sys.h"

typedef struct 
{
	float Kp;                     //比例常数
  float Ki;                     //积分常数
  float Kd;                     //微分常数
	float Target_value;           //目前值
	float Current_value;          //当前值
	float Err;                    //误差
	float Last_Err;               //上一次误差
	float integral;               //积分值
	float dErr;                   //微分值
	float output;                 //PID输出
	float output_max;             //输出最大值
	float output_min;             //输出最小值
	float max_integral;
}PID_TypeDef;

void PID_Init(PID_TypeDef*pid,float Kp,float Ki,float Kd,float max_integral,float output_max,float output_min);
s16 PID_operation(PID_TypeDef*pid,float Target_value,float Current_value);

#endif
