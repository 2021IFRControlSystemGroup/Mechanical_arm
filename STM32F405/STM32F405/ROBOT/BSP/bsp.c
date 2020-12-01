#include "bsp.h"
int BSP_OK_init_flag=0;

void BSP_Init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//����ϵͳ�ж����ȼ�����1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2 
	
	delay_init(168);	
	LED_Init();
	USART1_DMA_Config(10000);//ң������ʼ��
	TIM5_Int_Init(1000-1,84-1);
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,30,CAN_Mode_Normal);//CAN1��ʼ��
	delay_ms(1000);
	BSP_OK_init_flag=1;
}


