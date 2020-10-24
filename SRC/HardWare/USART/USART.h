

#ifndef _USART_H_
#define _USART_H_

#include "Public_StdTypes.h"

extern void USART1_Init(uint32_t Baudrate);       		
extern void USART_Send_Char(uint8_t ascii_code); 	

#endif


