#include "can2_analysis.h"
#include "can2.h"
#include "yun.h"
#include "protect.h"

extern CanRxMsg receive_data2;
extern ERROR_CHECK Error_Check;
YUN_DATA Yun_data[2];

void Receive_Deal_Yun(void)
{
	switch(receive_data2.StdId)
	{
	  case 0x205:
      Yun_data[0].position=(( receive_data2.Data[0]<<8)| receive_data2.Data[1]);
		  Yun_data[0].speed = (( receive_data2.Data[2]<<8)| receive_data2.Data[3]);
      Yun_data[0].torque=(( receive_data2.Data[4]<<8)| receive_data2.Data[5]);
      Yun_data[0].temperature=receive_data2.Data[6];
		  Absolute_Position_YUN_DATA(&Yun_data[Pitch]);
			LostCountFeed(&Error_Check.count[LOST_PITCH]);
	  case 0x206:
			Yun_data[1].position=(( receive_data2.Data[0]<<8)| receive_data2.Data[1]);
			Yun_data[1].speed = (( receive_data2.Data[2]<<8)| receive_data2.Data[3]);
		  Yun_data[1].torque=(( receive_data2.Data[4]<<8)| receive_data2.Data[5]);
      Yun_data[1].temperature=receive_data2.Data[6];
      Absolute_Position_YUN_DATA(&Yun_data[Yaw]);
			LostCountFeed(&Error_Check.count[LOST_YAW]);		
		break;
	}
}

u8 sendmag_yun[8]={0};
void Send_yun_Deal(s16 Yun0,s16 Yun1)
{
 	sendmag_yun[0]=(Yun0>>8)&0XFF;                    //输出高八位
	sendmag_yun[1]=Yun0&0XFF;                         //输出低八位
	sendmag_yun[2]=(Yun1>>8)&0XFF;
	sendmag_yun[3]=Yun1&0XFF; 
	sendmag_yun[4]=0x00; 
	sendmag_yun[5]=0x00; 
	sendmag_yun[6]=0x00;
	sendmag_yun[7]=0x00; 
	CAN2_Send_Msg(sendmag_yun,0x1FF);           //发送函数 
}

void Absolute_Position_YUN_DATA(YUN_DATA*yun_data)            //绝对位置函数
{
	static int flag=0;
	if(flag==0)
  { yun_data->Absolute_position=0 ; flag=flag+1; }
	else if(yun_data->position-yun_data->Last_position >= 4096)               //反向过界
	{ yun_data->Absolute_position=yun_data->Absolute_position-yun_data->Last_position+yun_data->position-8192; }
	else if(yun_data->position-yun_data->Last_position <= -4096)              //正向过界
	{ yun_data->Absolute_position=yun_data->Absolute_position+yun_data->position+8192-yun_data->Last_position; }
	else                                                                      //不过界
	{ yun_data->Absolute_position=yun_data->Absolute_position+yun_data->position-yun_data->Last_position;	}
	yun_data->Last_position  =yun_data->position; 
}

