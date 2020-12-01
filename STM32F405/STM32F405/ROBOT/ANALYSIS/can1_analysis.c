#include "can1_analysis.h"


int32_t data[2]={0};
void Receive_Deal_Yun(CanRxMsg *Rx_message)
{
	CAN_Receive(CAN1, CAN_FIFO0, Rx_message);//��ȡ����
	switch(Rx_message->RTR)
	{	
		case 0x01:
		{
//			data[0]=Rx_message->Data[0];
			break;
		}
		case 0x02:
		{
//			data[0]=(( Rx_message->Data[0]<<8)| Rx_message->Data[1]);
			break;
		}
		case 0x04:
		{
//			data[0]=(( Rx_message->Data[0]<<24)| Rx_message->Data[1]<<16)|(( Rx_message->Data[2]<<8)| Rx_message->Data[3]);
			break;
		}
		case 0x08:
		{
//			data[0]=(( Rx_message->Data[0]<<24)| Rx_message->Data[1]<<16)|(( Rx_message->Data[2]<<8)| Rx_message->Data[3]);
//			data[1]=(( Rx_message->Data[4]<<24)| Rx_message->Data[5]<<16)|(( Rx_message->Data[6]<<8)| Rx_message->Data[7]);
			break;
		}
		default:
			break;
	}
}

void Hand_Shake1(void)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x00;
	TxMessage.ExtId = 0x0008019E;
	TxMessage.IDE = CAN_Id_Extended;		
	TxMessage.RTR = CAN_RTR_Data;		  
	TxMessage.DLC = 0x02;							 

	TxMessage.Data[0] = 0x0A;
	TxMessage.Data[1] = 0x07;
	
	CAN_Transmit(CAN1, &TxMessage);   
}

void Hand_Shake2(void)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x00;
	TxMessage.ExtId = 0x0010019E;
	TxMessage.IDE = CAN_Id_Extended;		
	TxMessage.RTR = CAN_RTR_Data;		  
	TxMessage.DLC = 0x02;							 

	TxMessage.Data[0] = 0x0A;
	TxMessage.Data[1] = 0x07;
	
	CAN_Transmit(CAN1, &TxMessage);   
}

void Motor_Eable1(void)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x00;
	TxMessage.ExtId = 0x00080181;
	TxMessage.IDE = CAN_Id_Extended;		
	TxMessage.RTR = CAN_RTR_Data;		  
	TxMessage.DLC = 0x00;							 
	
	CAN_Transmit(CAN1, &TxMessage);   
}

void Motor_Eable2(void)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x00;
	TxMessage.ExtId = 0x00100181;
	TxMessage.IDE = CAN_Id_Extended;		
	TxMessage.RTR = CAN_RTR_Data;		  
	TxMessage.DLC = 0x00;							 
	
	CAN_Transmit(CAN1, &TxMessage);   
}


void Motor_Postion1(int32_t pos_tar)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x00;
	TxMessage.ExtId = 0x00080186;
	TxMessage.IDE = CAN_Id_Extended;		
	TxMessage.RTR = CAN_RTR_Data;		  
	TxMessage.DLC = 0x04;			

	TxMessage.Data[0] = (unsigned char)((pos_tar>>24)&0xff);
	TxMessage.Data[1] = (unsigned char)((pos_tar>>16)&0xff);
	TxMessage.Data[2] = (unsigned char)((pos_tar>>8)&0xff);
	TxMessage.Data[3] = (unsigned char)((pos_tar)&0xff);
	
	CAN_Transmit(CAN1, &TxMessage); 
}

void Motor_Postion2(int32_t pos_tar)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x00;
	TxMessage.ExtId = 0x00100186;
	TxMessage.IDE = CAN_Id_Extended;		
	TxMessage.RTR = CAN_RTR_Data;		  
	TxMessage.DLC = 0x04;			

	TxMessage.Data[0] = (unsigned char)((pos_tar>>24)&0xff);
	TxMessage.Data[1] = (unsigned char)((pos_tar>>16)&0xff);
	TxMessage.Data[2] = (unsigned char)((pos_tar>>8)&0xff);
	TxMessage.Data[3] = (unsigned char)((pos_tar)&0xff);
	
	CAN_Transmit(CAN1, &TxMessage); 
}

