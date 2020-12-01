#include "bsp.h"
#include "yun.h"
#include "can1_analysis.h"

YUN_DATA yun_data[2]={0};
PID_TypeDef GM6020_PID[2]={0};
PID_TypeDef POS_GM6020_PID[2]={0};
s32 yun_tar[2]={0};
int postion_tar = 2000;
int GM6020output_max=14000;
int GM6020output_min=-14000;
int GM6020integral_max=100000;

void yun(void)
{
//	yun_tar[0]=yun_data[0].Absolute_position+postion_tar;
	PID_operation(&POS_GM6020_PID[0],postion_tar,yun_data[0].Absolute_position);
	PID_operation(&GM6020_PID[0],POS_GM6020_PID[0].output,yun_data[0].speed);
	CAN1_Send_Yun(GM6020_PID[0].output);
}

void Yun_PID_Init(void)                       //pid参数初始化
{
	PID_Init(&POS_GM6020_PID[0],5,0.008,20,GM6020integral_max,GM6020output_max,GM6020output_min);
	PID_Init(&GM6020_PID[0],2,0,1.5,GM6020integral_max,GM6020output_max,GM6020output_min);
}



