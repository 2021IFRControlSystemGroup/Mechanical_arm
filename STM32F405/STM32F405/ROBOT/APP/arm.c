#include "arm.h"

float MAX_SPEED=10000.0f;

float pi=3.1415927f;

int CAN1_flag=0;
int CAN2_flag=0;
int Motor_Eable_flag=0;
int Arm_Init_flag=0;
int Catch_flag=0;
int hand_shake_flag=0;

float tar_X=0.0f;
float tar_Y=0.0f;
float now_angle1=0.0f;
float now_angle2=0.0f;
float tar_angle1=0.0f;
float tar_angle2=0.0f;
float tar_speed1=4000.0f;
float tar_speed2=4000.0f;
float L1=20.0f; 
float L2=17.0f;
float L3=7.0f;
int32_t pos_tar1=0;
int32_t pos_tar2=0;
int32_t pos_tar3=0;
int32_t pos_last1=0;
int32_t pos_last2=0;
//test
float center1=17.0f;
float center2=40.0f;
float R=30.0f;
u8 round_flag=1;
float angle1=0.0f;
float angle2=0.0f;
int num=31;
float angle=0.0f;
float step=0.0f;
int time_space=80;
extern uint32_t system_1ms;


void Arm_task(void)
{
	if(RC_Ctl.rc.switch_right==RC_SWITCH_UP)
	{
		time_space=80;
		tar_X+=(RC_Ctl.rc.ch0-1024.0f)/660.0f*1.8f;
		tar_Y+=(RC_Ctl.rc.ch1-1024.0f)/660.0f*1.8f;
	}
	else if(RC_Ctl.rc.switch_left==RC_SWITCH_UP&&round_flag>0)
	{		
		if(round_flag==1)time_space=1000;
		else time_space=80;
		step=(angle2-angle1)/(num-1);
		angle=angle1+(round_flag-1)*step;
				
		tar_X=center1+R*cos(angle);
		tar_Y=center2+R*sin(angle);
		
		round_flag++;
		if(round_flag>num)round_flag=0;
	}	
	else if(round_flag!=1&&RC_Ctl.rc.switch_left==RC_SWITCH_MIDDLE)
	{
		tar_X=0.0f;
		tar_Y=37.0f;
		round_flag=1;
	}
	
	Catch_flag=Arm_Workspace_Judge(tar_X,tar_Y);
	
	if(Catch_flag==0)
	{
		
	}
	else
	{
		Angle();
		pos_last1=pos_tar1;
		pos_last2=pos_tar2;
		pos_tar1=(int32_t)(tar_angle1/2.0f/pi*6400.0f*3.60f);
		pos_tar2=(int32_t)((-tar_angle2/2.0f/pi*6400.0f*3.86f)-tar_angle1/2.0f/pi*6400.0f*2.77f);
		
//		distance1=abs(pos_tar1-pos_last1);
//		distance2=abs(pos_tar2-pos_last2);
//		
//		tar_speed1=distance1*distance1/time_space*p;
//		tar_speed2=distance2*distance2/time_space*p;
//		
//		if(tar_speed1>MAX_SPEED)tar_speed1=MAX_SPEED;
//		if(tar_speed2>MAX_SPEED)tar_speed2=MAX_SPEED;
//		if(distance1==0)tar_speed1=MAX_SPEED;
//		if(distance2==0)tar_speed2=MAX_SPEED;
		
		Motor_Postion1(ID1_POS,pos_tar1,MAX_SPEED);
		Motor_Postion2(ID2_POS,pos_tar2,MAX_SPEED);
	}	
}

void Arm_Init(void)
{
	Round_Crossover_Point(center1,center2,L1+L2,R);
	if(CAN1_flag==1&&CAN2_flag==1)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_0);
		Motor_Calibrate1(ID1_ZERO);
		Motor_Calibrate2(ID2_ZERO);
		Arm_Init_flag++;
	}
	else
	{
		Hand_Shake1(ID1_HANDSHAKE);
		Hand_Shake2(ID2_HANDSHAKE);
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
//	else if(sqrt(tarX*tarX+tarY*tarY)<18.68)
//	{
//		return 0;
//	}
//	else if(tarY<L1*cos(2*pi/3))
//	{
//		return 0;
//	}
	return 1;
}


void Round_Crossover_Point(float center1,float center2,float R,float r)
{
	float x0=0.0f;
  float y0=0.0f;
  float r0=R;

  float x1=center1;
  float y1=center2;
  float r1=r;

  float d=sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1));   

  float k1=(y0-y1)/(x0-x1);        
  float b1=y1-k1*x1;

  float k2=-1.0f/k1;              
  float b2=(r0*r0-r1*r1-x0*x0+x1*x1-y0*y0+y1*y1)/(2*(y1-y0));

  if(fabs(r1-r0)<d && d<r1+r0)      
	{   
		float xx1=(-b2* k2 + x1 + k2 *y1 - sqrt(-b2*b2 + r1*r1 + k2*k2*r1*r1 - 2 *b2* k2* x1 - k2*k2* x1*x1 + 2*b2*y1 + 2*k2*x1*y1 - y1*y1))/(1 + k2*k2);
    float yy1=k2*xx1+b2;
     
    float xx2=(-b2* k2 + x1 + k2 *y1 + sqrt(-b2*b2 + r1*r1 + k2*k2*r1*r1 - 2 *b2* k2* x1 - k2*k2* x1*x1 + 2*b2*y1 + 2*k2*x1*y1 - y1*y1))/(1 + k2*k2);  
    float yy2=k2*xx2+b2;

		if(yy2>yy1)
		{
			angle1=atan2(yy2-center2,xx2-center1);
			angle2=atan2(yy1-center2,xx1-center1);
		}
		else
		{
			angle1=atan2(yy1-center2,xx1-center1);
			angle2=atan2(yy2-center2,xx2-center1);
		}		
	}
	if(angle1-angle2>pi)
  angle1=angle1-2*pi;  
	if(angle1-angle2<-pi)
  angle1=angle1+2*pi; 
}

