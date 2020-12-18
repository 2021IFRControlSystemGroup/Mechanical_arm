#include "arm.h"

float pi=3.1415927f;

int CAN_flag=0;
int Motor_Eable_flag=0;
int Arm_Init_flag=0;
int Catch_flag=0;
int hand_shake_flag=0;

float tar_X=0.0f;
float tar_Y=37.0f;
float now_angle1=0.0f;
float now_angle2=0.0f;
float tar_angle1=0.0f;
float tar_angle2=0.0f;
float L1=20.0f; 
float L2=17.0f;
float L3=7.0f;
int32_t pos_tar1=0;
int32_t pos_tar2=0;
int32_t pos_tar3=0;
u8 ID_flag=1;


extern uint32_t system_1ms;
void Arm_task(void)
{
	tar_X+=(RC_Ctl.rc.ch0-1024)/660.0f*0.1f;
	tar_Y+=(RC_Ctl.rc.ch1-1024)/660.0f*0.1f;
	if(tar_X<-37.0f)
		tar_X=-37.0f;
	else if(tar_X>37.0f)
		tar_X=37.0f;
	if(tar_Y<0.0f)
		tar_Y=0.0f;
	else if(tar_Y>37.0f)
		tar_Y=37.0f;
	Catch_flag=Arm_Workspace_Judge(tar_X,tar_Y);
	if(Catch_flag==0)
	{
		
	}
	else
	{
		Angle();
		pos_tar1=(int32_t)(tar_angle1/2.0f/pi*6400.0f*3.60f);
		pos_tar2=(int32_t)((-tar_angle2/2.0f/pi*6400.0f*3.86f)-tar_angle1/2.0f/pi*6400.0f*2.80f);
		
		if(ID_flag==1)
		{
			Motor_Postion(ID1_POS,pos_tar1);
			ID_flag=2;
		}
		else if(ID_flag==2)
		{
			Motor_Postion(ID2_POS,pos_tar2);
			ID_flag=1;
		}
//		else
//		{
//			Motor_Postion(ID3_POS,pos_tar3);
//			ID_flag=1;
//		}
	}	
}

void Arm_Init(void)
{
	if(CAN_flag==1&&hand_shake_flag>20)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_0);
		if(ID_flag==1)
		{
			Motor_Calibrate(ID1_ZERO);
			ID_flag=2;
			Arm_Init_flag++;
		}
		else if(ID_flag==2)
		{
			Motor_Calibrate(ID2_ZERO);
			ID_flag=1;
			Arm_Init_flag++;
		}
//		else
//		{
//			Motor_Calibrate(ID3_ZERO);
//			ID_flag=1;
//		}
	}
	else
	{
		if(ID_flag==1)
		{
			Hand_Shake(ID1_HANDSHAKE);
			ID_flag=2;
			hand_shake_flag++;
		}
		else if(ID_flag==2)
		{
			Hand_Shake(ID2_HANDSHAKE);
			ID_flag=1;
			hand_shake_flag++;
		}
//		else
//		{
//			Hand_Shake(ID3_HANDSHAKE);
//			ID_flag=1;
//		}
	}
}

void Angle(void)
{
	if(fabs(sqrt(tar_X*tar_X+tar_Y*tar_Y)-(L1+L2))<1e-8)
	{
		tar_angle1=pi/2-atan2(tar_Y,tar_X);
		tar_angle2=0.0f;
	}
	else 
	{
		tar_angle1=pi/2-atan2(tar_Y,tar_X)-acos((tar_X*tar_X+tar_Y*tar_Y+L1*L1-L2*L2)/(2*L1*sqrt(tar_X*tar_X+tar_Y*tar_Y)));
		tar_angle2=pi-acos((L1*L1+L2*L2-tar_X*tar_X-tar_Y*tar_Y)/(2*L1*L2));
	}
	if(tar_angle1-now_angle1>pi)
	{
		tar_angle1=tar_angle1-2*pi;
	}
	else if(tar_angle1-now_angle1<-pi)
	{
		tar_angle1=tar_angle1+2*pi;
	}
	
	if(tar_angle2-now_angle2>pi)
	{
		tar_angle2=tar_angle2-2*pi;
	}
	else if(tar_angle2-now_angle2<-pi)
	{
		tar_angle2=tar_angle2+2*pi;
	}
	
	if(tar_angle1>2*pi/3)
	{
		tar_angle1=2*pi/3;
	}
	if(tar_angle1<-2*pi/3)
	{
		tar_angle1=-2*pi/3;
	}
	if(tar_angle2>2*pi/3)
	{
		tar_angle2=2*pi/3;
	}
	if(tar_angle2<-2*pi/3)
	{
		tar_angle2=-2*pi/3;
	}
	
	now_angle1=tar_angle1;
	now_angle2=tar_angle2;
}



int Arm_Workspace_Judge(float tarX,float tarY)
{
	if(sqrt(tarX*tarX+tarY*tarY)>(L1+L2))
	{
		return 0;		
	}
	else if(sqrt(tarX*tarX+tarY*tarY)<18.68)
	{
		return 0;
	}
	else if(tarY<L1*cos(2*pi/3))
	{
		return 0;
	}
	return 1;
}



