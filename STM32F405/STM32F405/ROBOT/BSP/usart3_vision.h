#ifndef __USART3_VISION_H__
#define __USART3_VISION_H__
#include "stm32f4xx.h"


#define DMA_VISION_RX_BUF_LENGTH    100u //���޸�


typedef  struct
{
	u8 armor_sign;	//�Ƿ�����Чװ��
	u8 buff_sign;   
	u8 armor_type;	//װ������
	u16 armor_dis_or_buff_cy;	//�����Ϣ
	float tar_x;	//x����
	float tar_y;	//y����
	s16 Velocity_x_or_buff_cx; //װ�װ��˶��ٶ�
	float angel_x_v;	//���ںϵĵ�������ٶ�	//��λΪ0.05��ÿ��
	float angle_x_v_filter;
	u8 control_state;
	u8 Num;
	u8 runtime;
	
}VISION_DATA;



void USART3_Vision_Init(void);
void USART3_IRQHandler(void);
void VisionData_analysis(uint8_t *pData);

#define VISION_DATA_DEFAULT  {0,0,0,0,0,0,0,0,0,0,0,0};
#endif
