#ifndef __USART3_VISION_H__
#define __USART3_VISION_H__
#include "stm32f4xx.h"


#define DMA_VISION_RX_BUF_LENGTH    100u //待修改


typedef  struct
{
	u8 armor_sign;	//是否有有效装甲
	u8 buff_sign;   
	u8 armor_type;	//装甲类型
	u16 armor_dis_or_buff_cy;	//深度信息
	float tar_x;	//x坐标
	float tar_y;	//y坐标
	s16 Velocity_x_or_buff_cx; //装甲板运动速度
	float angel_x_v;	//经融合的到的相对速度	//单位为0.05度每秒
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
