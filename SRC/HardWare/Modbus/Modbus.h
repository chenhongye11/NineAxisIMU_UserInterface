#include "stm32f10x.h"
typedef struct
{
	uint8_t Address;
	uint8_t PitchH;
	uint8_t PitchL;
  uint8_t YawH;
	uint8_t YawL;
  uint8_t RollH;
	uint8_t RollL;
} TModbusRegister;
extern TModbusRegister ModbusRegister;
extern const uint8_t CRCHighArray[];
extern const uint8_t CRCLowArray[];

extern uint8_t CRCIndex;
extern uint8_t CRCHigh;
extern uint8_t CRCLow;

extern uint8_t RxBufferSize;
extern uint8_t TxBufferSize;

extern uint8_t RxBuffer[];
extern uint8_t RxBufferIndex;
extern uint8_t IsRxDataReady;

extern uint8_t TxBuffer[];
extern uint8_t TxBufferIndex;

extern void Modbus_Init(void);
extern void ModbusReceive(void);
extern void ModbusTransmit(void);
extern void ModbusIdle(void);
