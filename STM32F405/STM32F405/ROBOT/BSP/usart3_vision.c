#include "usart3_vision.h"


uint8_t   USART3_DMA_RX_BUF[2][DMA_VISION_RX_BUF_LENGTH] = {0};
uint8_t Vision_frame_rx_len = 0;
extern u32 system_1ms;
VISION_DATA Vision_Data = VISION_DATA_DEFAULT;


void USART3_Vision_Init(void)
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;
	NVIC_InitTypeDef nvic;
	DMA_InitTypeDef dma;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); 

	gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_AF;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&gpio);

	nvic.NVIC_IRQChannel = USART3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 2;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&nvic);

	USART_StructInit(&usart);
	usart.USART_BaudRate = 115200;
	usart.USART_WordLength = USART_WordLength_8b; 
	usart.USART_StopBits = USART_StopBits_1; 
	usart.USART_Parity = USART_Parity_Even;  
	usart.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;  
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 

	USART_Init(USART3, &usart);  
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);     
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);  
	USART_Cmd(USART3, ENABLE); 

	//相应的DMA配置
	DMA_DeInit(DMA1_Stream1);
	dma.DMA_Channel = DMA_Channel_4;
	dma.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);  //DMA外设ADC基地址
	dma.DMA_Memory0BaseAddr = (uint32_t)&USART3_DMA_RX_BUF[0][0]; //DMA内存基地址
	dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
	dma.DMA_BufferSize = DMA_VISION_RX_BUF_LENGTH;
	dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	dma.DMA_Mode = DMA_Mode_Circular;
	dma.DMA_Priority = DMA_Priority_Medium;   //DMA通道 x拥有中优先级 
	dma.DMA_FIFOMode = DMA_FIFOMode_Disable;   //DMA通道x没有设置为内存到内存传输
	dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream1, &dma);

	DMA_DoubleBufferModeConfig(DMA1_Stream1, (uint32_t)&USART3_DMA_RX_BUF[0][0], DMA_Memory_0);   //first used memory configuration
	DMA_DoubleBufferModeCmd(DMA1_Stream1, ENABLE);  
	DMA_Cmd(DMA1_Stream1, ENABLE);
	

}

u32 cs_time = 0;
u32 last_time = 0;

void USART3_IRQHandler(void)
{
	 if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	 {
		  (void)USART3->SR;
		  (void)USART3->DR;
			if(DMA_GetCurrentMemoryTarget(DMA1_Stream1) == 0)
		  {
				 Vision_frame_rx_len = DMA_VISION_RX_BUF_LENGTH - DMA_GetCurrDataCounter(DMA1_Stream1);				
			   DMA_Cmd(DMA1_Stream1, DISABLE);			
			   DMA1_Stream1->NDTR = (uint16_t)DMA_VISION_RX_BUF_LENGTH;    			
			   DMA_DoubleBufferModeConfig(DMA1_Stream1, (uint32_t)&USART3_DMA_RX_BUF[1][0], DMA_Memory_1); 
			   DMA_Cmd(DMA1_Stream1, ENABLE);
				 
				 VisionData_analysis(USART3_DMA_RX_BUF[0]);				
		  }
			else 
		  {
				 Vision_frame_rx_len = DMA_VISION_RX_BUF_LENGTH - DMA_GetCurrDataCounter(DMA1_Stream1);				
			   DMA_Cmd(DMA1_Stream1, DISABLE);
			   DMA1_Stream1->NDTR = (uint16_t)DMA_VISION_RX_BUF_LENGTH;      //??relocate the dma memory pointer to the beginning position`
			   DMA_DoubleBufferModeConfig(DMA1_Stream1, (uint32_t)&USART3_DMA_RX_BUF[0][0], DMA_Memory_0); 
			   DMA_Cmd(DMA1_Stream1, ENABLE);
				
				 VisionData_analysis(USART3_DMA_RX_BUF[1]);				
		  }
	 }
}

void VisionData_analysis(uint8_t *pData)
{
	int i = 0;
	int tar_x_raw = 0;
	int tar_y_raw = 0;	
	
	if(pData == 0)
	{
		return;
	}
  
	while(i<Vision_frame_rx_len)
	{
		while(pData[i] != 0x5a)
		{
			i++;
			if(i>=Vision_frame_rx_len) break;
		}
		
		
			cs_time = system_1ms - last_time;
			last_time = system_1ms;
	
		  Vision_Data.buff_sign = pData[i+1]>>4;
			Vision_Data.armor_sign = pData[i+1]>>7;
			Vision_Data.armor_dis_or_buff_cy = ((pData[i+2]<<8)|pData[i+3]) & 0xffff;
			tar_x_raw = ((pData[i+4]<<8)|pData[i+5]) & 0xffff;
			tar_y_raw = ((pData[i+6]<<8)|pData[i+7]) & 0xffff;	
			Vision_Data.Velocity_x_or_buff_cx = ((pData[i+8]<<8)|pData[i+9]) & 0xffff;
			
			Vision_Data.tar_x = tar_x_raw/10.0f;
			Vision_Data.tar_y = tar_y_raw/10.0f;	

			Vision_Data.runtime = pData[i+10];

			Vision_Data.Num++;
			if(Vision_Data.Num > 200)
			{
				Vision_Data.Num = 0;
			}
			i = i + 11;			
	}
	 
		
}

