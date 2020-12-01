#ifndef __DMA2_H
#define __DMA2_H
#include "bsp.h"


#define  USART1_DMA_RC_BUF_LEN               30u  
#define  RC_FRAME_LENGTH                     20u

void USART1_IRQHandler(void);
void USART1_DMA_Config(uint32_t baudrate);

#endif


