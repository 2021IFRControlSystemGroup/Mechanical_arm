#include "time2.h"
#include "can1_analysis.h"
#include "arm.h"
#include "delay.h"
#include "can2_analysis.h"

extern int CAN_flag;
extern int Motor_Eable_flag;
extern int32_t pos_tar;
extern int BSP_OK_init_flag;
extern int Arm_Init_flag;
extern int32_t pos_tar1;
extern int32_t pos_tar2;
extern RC_Ctl_t RC_Ctl;
uint32_t system_1ms=0;

void TIM5_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///ʹ��TIM2ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//��ʼ��TIM2
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM5,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
extern int time_space;
//��ʱ��3�жϷ�����
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)!= RESET) //����ж�
	{
		system_1ms++;
		if(RC_Ctl.rc.switch_left==RC_SWITCH_DOWN)
		{
			Motor_Eable1(ID1_ENABLE+1);
			Motor_Eable2(ID2_ENABLE+1);
		}
		if(RC_Ctl.rc.switch_right==RC_SWITCH_DOWN)
		{
			Arm_Init_flag=0;
			GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		}
		static int zero_time=0;
		if(BSP_OK_init_flag)	
		{
			if(system_1ms>1000)
			{
				if(system_1ms%100==0)
				{
					if(Arm_Init_flag==0)
					{
						Arm_Init();		
            zero_time=system_1ms;						
					}	
				}
				if(system_1ms-zero_time>3000&&system_1ms%time_space==0)
				{
					Arm_task();
				}
			}			
		}
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //����жϱ�־λ
}

