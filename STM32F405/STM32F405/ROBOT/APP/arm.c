#include "arm.h"

float pi=3.1415927f;

int CAN_flag=0;
int Motor_Eable_flag=0;
int Arm_Init_flag=0;
int Catch_flag=0;

float tar_X=0.0f;
float tar_Y=0.0f;
float now_angle1=0.0f;
float now_angle2=0.0f;
float tar_angle1=0.0f;
float tar_angle2=0.0f;
float L1=20.0f;
float L2=17.0f;
int32_t pos_tar1=0;
int32_t pos_tar2=0;
int32_t pos_fdb1=0;
int32_t pos_fdb2=0;
extern uint32_t system_1ms;

void Arm_task(void)
{
	Arm_Workspace_Judge();
	if(Catch_flag==0)
	{
		
	}
	else
	{
		Angle();
		pos_tar1=(int32_t)(tar_angle1/2/pi*6400*3.60f)+pos_fdb1;
		pos_tar2=(int32_t)(tar_angle2/2/pi*6400*3.86f)+pos_fdb2;
		Motor_Postion1(pos_tar1);
		Motor_Postion2(pos_tar2);
	}	
}

void Arm_Init(void)
{
	if(CAN_flag==1)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_0);
		Motor_Eable1();
		Motor_Eable2();
		Motor_Eable_flag++;
	}
	else
	{
		Hand_Shake1();
		Hand_Shake2();
	}
	if(Motor_Eable_flag>1000)
	{
		Arm_Init_flag=1;
		Motor_Calibrate();
	}
}

void Angle(void)
{
	if(fabs(sqrt(tar_X*tar_X+tar_Y*tar_Y)-(L1+L2))<1e-8)
	{
		tar_angle1=atan2(tar_Y,tar_X);
		tar_angle2=0.0f;
	}
	else
	{
			tar_angle1=atan2(tar_Y,tar_X)-acos((tar_X*tar_X+tar_Y*tar_Y+L1*L1-L2*L2)/(2*L1*sqrt(tar_X*tar_X+tar_Y*tar_Y)));
			tar_angle2=pi-acos((L1*L1-L2*L2-tar_X*tar_X-tar_Y*tar_Y)/(2*L1*L2));
	}
	
	if(tar_angle1-now_angle1>=pi)
	{
		tar_angle1=tar_angle1-2*pi;
	}
	else if(tar_angle1-now_angle1<=-pi)
	{
		tar_angle1=tar_angle1+2*pi;
	}
	
	if(tar_angle2-now_angle2>=pi)
	{
		tar_angle2=tar_angle2-2*pi;
	}
	else if(tar_angle2-now_angle2<=-pi)
	{
		tar_angle2=tar_angle2+2*pi;
	}
	
	now_angle1=tar_angle1;
	now_angle2=tar_angle2;
}

void Motor_Calibrate(void)
{
	




}

void Arm_Workspace_Judge(void)
{




}


