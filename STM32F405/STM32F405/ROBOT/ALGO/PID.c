#include "PID.h"

extern PID_TypeDef M3508_PID[4];

void PID_Init(PID_TypeDef*pid,float Kp,float Ki,float Kd,float max_integral,float output_max,float output_min)
{
  pid->Kp=Kp;
	pid->Ki=Ki;
	pid->Kd=Kd;
	pid->output_min=output_min;
	pid->output_max=output_max;	
  pid->max_integral=max_integral;
}

//pid操作
s16 PID_operation(PID_TypeDef*pid,float Target_value,float Current_value)
{
//	PID.Target_value = value;  //确定目标值
	pid->Err=Target_value-Current_value;//求误差
	pid->integral += pid->Err;//积分
	if(pid->integral > pid->max_integral) pid->integral = pid->max_integral;
	if(pid->integral < -pid->max_integral) pid->integral = -pid->max_integral;
	pid->dErr=pid->Err-pid->Last_Err;//微分
	pid->output=pid->Kp*pid->Err+pid->Ki*pid->integral+pid->Kd*pid->dErr;//pid计算
	pid->Last_Err=pid->Err;//将当前误差值存入上一误差值
	if(pid->output < pid->output_min)	{pid->output =pid->output_min;}
	if(pid->output > pid->output_max)	{pid->output =pid->output_max;}   //限制输出
	
	return pid->output;
}




