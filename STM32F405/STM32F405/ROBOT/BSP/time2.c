#include "time2.h"
#include "can1_analysis.h"
#include "arm.h"
#include "delay.h"


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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///使能TIM2时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//初始化TIM2
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM5,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
//定时器3中断服务函数
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)!= RESET) //溢出中断
	{
		system_1ms++;
		static int zero_time=0;
		if(BSP_OK_init_flag)	
		{
			if(system_1ms>3000)
			{
				if(system_1ms%100==0)
				{
					if(Arm_Init_flag<4)
					{
						Arm_Init();		
            zero_time=system_1ms;						
					}	
				}
				if(system_1ms-zero_time>5000&&system_1ms%10==0)
				{
					Arm_task();
				}
			}			
		}
//		if(Arm_Init_flag==0)
//		{
//			Arm_Init();			
//		}
//		else
//		{
//			if(system_1ms%500==0)
//			{
//				if(system_1ms%750==0)
//				{
//					Motor_Postion1(pos_tar1);			
//				}
//					Motor_Postion2(pos_tar2);			
//			}
//			
//		}
//		
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //清除中断标志位
}

