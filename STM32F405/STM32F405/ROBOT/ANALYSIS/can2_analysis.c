#include "can2_analysis.h"
#include "can2.h"


void Hand_Shake2(uint32_t ID)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x00;
	TxMessage.ExtId = ID;
	TxMessage.IDE = CAN_Id_Extended;		
	TxMessage.RTR = CAN_RTR_Data;		  
	TxMessage.DLC = 0x02;							 

	TxMessage.Data[0] = 0x0A;
	TxMessage.Data[1] = 0x07;
	
	CAN_Transmit(CAN2, &TxMessage);   
}

void Motor_Eable2(uint32_t ID)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x00;
	TxMessage.ExtId = ID;
	TxMessage.IDE = CAN_Id_Extended;		
	TxMessage.RTR = CAN_RTR_Data;		  
	TxMessage.DLC = 0x00;							 
	
	CAN_Transmit(CAN2, &TxMessage);   
}


void Motor_Postion2(uint32_t ID,int32_t pos_tar,float speed_tar)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x00;
	TxMessage.ExtId = ID;
	TxMessage.IDE = CAN_Id_Extended;		
	TxMessage.RTR = CAN_RTR_Data;		  
	TxMessage.DLC = 0x08;			

	uint32_t speed = *((uint32_t*)(&speed_tar));
	
	TxMessage.Data[0] = (unsigned char)((pos_tar>>24)&0xff);
	TxMessage.Data[1] = (unsigned char)((pos_tar>>16)&0xff);
	TxMessage.Data[2] = (unsigned char)((pos_tar>>8)&0xff);
	TxMessage.Data[3] = (unsigned char)((pos_tar)&0xff);
	TxMessage.Data[4] = (unsigned char)((speed>>24)&0xff);
	TxMessage.Data[5] = (unsigned char)((speed>>16)&0xff);
	TxMessage.Data[6] = (unsigned char)((speed>>8)&0xff);
	TxMessage.Data[7] = (unsigned char)((speed)&0xff);
	
	CAN_Transmit(CAN2, &TxMessage); 
}

void Motor_Calibrate2(uint32_t ID)
{
	CanTxMsg TxMessage;
	TxMessage.StdId = 0x00;
	TxMessage.ExtId = ID;
	TxMessage.IDE = CAN_Id_Extended;		
	TxMessage.RTR = CAN_RTR_Data;		  
	TxMessage.DLC = 0x00;							 
	
	CAN_Transmit(CAN2, &TxMessage); 
}





