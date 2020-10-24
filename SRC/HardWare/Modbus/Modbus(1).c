
#include "Modbus.h"
#include "IMU.h"
uint8_t CRCIndex = 0;
uint8_t CRCHigh = 0xFF;
uint8_t CRCLow = 0xFF;

TModbusRegister ModbusRegister;

uint8_t RxBufferSize = 8;
uint8_t TxBufferSize = 20;
uint8_t RxBuffer[8];
uint8_t RxBufferIndex = 0;
uint8_t IsRxDataReady = 0;

uint8_t TxBuffer[20];
uint8_t TxBufferIndex = 0;



void Modbus_Init(void)
{		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_TimeBaseStructure.TIM_Period = 1000-1;//1000-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
			
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//占先式优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
	NVIC_Init(&NVIC_InitStructure);//中断初始化	
	
	TIM_Cmd(TIM3, DISABLE); 
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);		
  
	
	ModbusRegister.Address = 0x01;
}

void ModbusReceive(void)
{
	TIM_SetCounter(TIM3, 0x0000); 
	if((TIM3->CR1 & 0x0001) != 0x0001) 
	{
		TIM_Cmd(TIM3, ENABLE);
	}
	IsRxDataReady = 0;	
	if(RxBufferIndex < RxBufferSize)
	{
 		RxBuffer[RxBufferIndex] = USART_ReceiveData(USART1);
		RxBufferIndex += 1;
  }
	else
	{
		RxBufferIndex = 0;
		RxBuffer[RxBufferIndex] = USART_ReceiveData(USART1);
		RxBufferIndex += 1;
	}
	
}
void ModbusTransmit(void)
{   
	USART_SendData(USART1, TxBuffer[TxBufferIndex]);	
	TxBufferIndex = TxBufferIndex + 1;
	if(TxBufferIndex >= TxBufferSize)//发送数据完成
  {   
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE); //关闭发送中断
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); 
		TxBufferIndex = 0;
	}  		 
}
void ModbusIdle(void)
{
	if(RxBufferIndex == RxBufferSize)
	{
		IsRxDataReady = 1;
	}
	else
	{
		IsRxDataReady = 0;		
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);	
	}
	RxBufferIndex = 0;
	if(IsRxDataReady == 1)
	{
		uint8_t index = 0;		 		
		CRCIndex = 0;
		CRCHigh = 0x00;
		CRCLow = 0x00;	
		
		if(!((CRCLow == 0x00) && (CRCHigh == 0x00))) return;
		if(RxBuffer[0] != ModbusRegister.Address) return;

		if(RxBuffer[1] == 0x04 && RxBuffer[5] < 15)
		{
			TxBuffer[0] = ModbusRegister.Address;
			TxBuffer[1] = 0x04;
			TxBuffer[2] = 0x06;
			TxBuffer[3] = ModbusRegister.PitchH;
			TxBuffer[4] = ModbusRegister.PitchL;	
			TxBuffer[5] = ModbusRegister.YawH;
			TxBuffer[6] = ModbusRegister.YawL;	
			TxBuffer[7] = ModbusRegister.RollH;
			TxBuffer[8] = ModbusRegister.RollL;	
			CRCIndex = 0;
			CRCHigh = 0xFF;
			CRCLow = 0xFF;
							
			TxBuffer[9] = CRCLow;
			TxBuffer[10] = CRCHigh;
			TxBufferSize = 11;
			TxBufferIndex = 0;
			USART_ITConfig(USART1,USART_IT_RXNE,DISABLE); 
			USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	
			//USARTx->CR1
		}
	}
}


//typedef struct
//{
//	unsigned char velocityH;
//	unsigned char velocityL;
//	unsigned char temperatureH;
//	unsigned char temperatureL;
//	unsigned char address;
//} TModbusRegister;
