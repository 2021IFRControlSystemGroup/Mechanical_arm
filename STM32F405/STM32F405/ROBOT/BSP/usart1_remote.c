#include "usart1_remote.h"
#include "remote_analysis.h"

//extern ERROR_CHECK Error_Check;


uint8_t   USART1_DMA_RC_BUF[2][USART1_DMA_RC_BUF_LEN] = {0};
uint8_t  Remote_frame_rx_len = 0;

void USART1_Remote_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  USART_InitTypeDef usart1;
  NVIC_InitTypeDef nvic;
	DMA_InitTypeDef dma;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_DeInit(USART1);
	USART_StructInit(&usart1);
	usart1.USART_BaudRate = 100000;
	usart1.USART_WordLength = USART_WordLength_8b;
	usart1.USART_StopBits = USART_StopBits_1;
	usart1.USART_Parity = USART_Parity_Even;
	usart1.USART_Mode = USART_Mode_Rx ;
	usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1,&usart1);

	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); 
	USART_Cmd(USART1,ENABLE);	
	
  nvic.NVIC_IRQChannel = USART1_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 1;	//2
  nvic.NVIC_IRQChannelSubPriority = 1;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
	
  DMA_DeInit(DMA2_Stream5);
  dma.DMA_Channel = DMA_Channel_4;
  dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
  dma.DMA_Memory0BaseAddr = (uint32_t)&USART1_DMA_RC_BUF[0][0];
  dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
  dma.DMA_BufferSize = USART1_DMA_RC_BUF_LEN;
  dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  dma.DMA_Mode = DMA_Mode_Circular;
  dma.DMA_Priority = DMA_Priority_VeryHigh;
  dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
  dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream5, &dma);

  DMA_DoubleBufferModeConfig(DMA2_Stream5, (uint32_t)&USART1_DMA_RC_BUF[0][0], DMA_Memory_0);   //first used memory configuration
  DMA_DoubleBufferModeCmd(DMA2_Stream5, ENABLE);  
  DMA_Cmd(DMA2_Stream5, ENABLE);

}

//DMA2数据流2中断服务函数
void USART1_IRQHandler(void)
{ 
  if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
  {
		  (void)USART1->SR;
		  (void)USART1->DR;
			if(DMA_GetCurrentMemoryTarget(DMA2_Stream5) == 0)
		  {
				 Remote_frame_rx_len = USART1_DMA_RC_BUF_LEN - DMA_GetCurrDataCounter(DMA2_Stream5);				
			   DMA_Cmd(DMA2_Stream5, DISABLE);			
			   DMA2_Stream5->NDTR = (uint16_t)USART1_DMA_RC_BUF_LEN;    			
			   DMA_DoubleBufferModeConfig(DMA2_Stream5, (uint32_t)&USART1_DMA_RC_BUF[1][0], DMA_Memory_1); 
			   DMA_Cmd(DMA2_Stream5, ENABLE);
				 
				 if(Remote_frame_rx_len>=18)
				 {
//						LostCountFeed(&Error_Check.count[LOST_DBUS]);					 
				    RemoteData_analysis(USART1_DMA_RC_BUF[0]);
				 }
				
		  }
			else 
		  {
				 Remote_frame_rx_len = USART1_DMA_RC_BUF_LEN - DMA_GetCurrDataCounter(DMA2_Stream5);				
			   DMA_Cmd(DMA2_Stream5, DISABLE);
			   DMA2_Stream5->NDTR = (uint16_t)USART1_DMA_RC_BUF_LEN;   
			   DMA_DoubleBufferModeConfig(DMA2_Stream5, (uint32_t)&USART1_DMA_RC_BUF[0][0], DMA_Memory_0); 
			   DMA_Cmd(DMA2_Stream5, ENABLE);
				
				 if(Remote_frame_rx_len>=18)	
				 {					 
//						LostCountFeed(&Error_Check.count[LOST_DBUS]);					 
						RemoteData_analysis(USART1_DMA_RC_BUF[1]);
				 }					 
		  }
//		LostCountFeed(&Error_Check.count[LOST_DBUS]);
	}
}


