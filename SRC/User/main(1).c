

#include "System.h"

#include "stm32f10x.h"

#include "stm32f10x_tim.h"
#include "Modbus.h"

int16_t TestmagnBuff[9]={0};
bool Flag_Check_Magn= false; 
void TestMagn(void); 

//#define RxBufferSize 15
//#define TxBufferSize 10

//uint8_t RxBuffer[RxBufferSize];
//uint8_t TxBuffer[TxBufferSize];


u16 count_out;
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;                    //定义外部高速晶体启动状态枚举变量
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_DeInit();                                    //复位RCC外部设备寄存器到默认值
  RCC_HSEConfig(RCC_HSE_ON);                       //打开外部高速晶振
  HSEStartUpStatus = RCC_WaitForHSEStartUp();      //等待外部高速时钟准备好
  if(HSEStartUpStatus == SUCCESS)                  //外部高速时钟已经准别好
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //开启FLASH预读缓冲功能，加速FLASH的读取。所有程序中必须的用法.位置：RCC初始化子函数里面，时钟起振之后
    FLASH_SetLatency(FLASH_Latency_2);                    //flash操作的延时
      	
    RCC_HCLKConfig(RCC_SYSCLK_Div1);               //配置AHB(HCLK)时钟等于==SYSCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);                //配置APB2(PCLK2)钟==AHB时钟
    RCC_PCLK1Config(RCC_HCLK_Div2);                //配置APB1(PCLK1)钟==AHB1/2时钟
         
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //配置PLL时钟 == 外部高速晶体时钟 * 9 = 72MHz
    RCC_PLLCmd(ENABLE);                                   //使能PLL时钟
   
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)    //等待PLL时钟就绪
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //配置系统时钟 = PLL时钟
    while(RCC_GetSYSCLKSource() != 0x08)                  //检查PLL时钟是否作为系统时钟
    {
    }
  }
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //允许AFIO时钟

   /* Enable GPIOA, GPIOD and USART1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);  
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 ;
	//Zigbee port
	GPIO_InitStructure.GPIO_Pin = GPIO_InitStructure.GPIO_Pin | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8; 
	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
}
int main(void)
{
	/*!< At this stage the microcontroller clock setting is already configured, 
	   this is done through SystemInit() function which is called from startup
	   file (startup_stm32f10x_xx.s) before to branch to application main.
	   To reconfigure the default setting of SystemInit() function, refer to
	   system_stm32f10x.c file
	   */ 
	//float Angle;
//			uint8_t i = 0;

	
	System_Init();	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);
 
	
//	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
//    //LedGpioConfiguration();
//    //JoystickGpioConfiguration();
	
	RCC_Configuration();
//  Set_USBClock();
//  USB_Interrupts_Config();
//  USB_Init();
	
 Modbus_Init();
	
//	//ZigBee SLEEP = 1 WAKE = 1 DEF = 1 RESET = 1
//	
//  GPIO_SetBits(GPIOB, (GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8));	
//	Delay_ms(10);
//	
//		//ZigBee reset = 0
//	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
//	Delay_ms(20);
//	
//	GPIO_SetBits(GPIOB, GPIO_Pin_4);
//	Delay_ms(20);
	
	
		
	while(1)
	{	

////		if(!Flag_Check_Magn)
////		{
////			if(KyeAndJoystickScan())
////			TestMagn();
////			LedShow();	
////		}
		if(IMU_SampleFlag)
		{
			IMU_GetYawPitchRoll(angles);	
			IMU_SampleFlag=0;
		}

//		if(UART_UpdataFlag)
//		{		    
//			printf("\r\n /2222222222222222222222222222222222222222222222/ \r\n");
//			printf("\r\n 2Roll: %.2f     Pitch: %.2f     Yaw: %.2f \r\n",angles[2],angles[1],angles[0]);
//			printf("\r\n 2Acceleration: X: %d     Y: %d     Z: %d \r\n",accel[0],accel[1],accel[2]);
//			printf("\r\n 2Gyroscope: X: %d     Y: %d     Z: %d \r\n",gyro[0],gyro[1],gyro[2]);
// 			printf("\r\n 2Magnetic: X: %d     Y: %d     Z: %d \r\n",magn[0],magn[1],magn[2]);

//			UART_UpdataFlag = 0;
//		}
	}
}
/**
  * @brief  check magn
  * @param  None
  * @retval  none
  */
void TestMagn(void)
{
//	TestmagnBuff[(KeyAndJostickValue-1)*3]=magn[0];
//	TestmagnBuff[(KeyAndJostickValue-1)*3+1]=magn[1];
//	TestmagnBuff[(KeyAndJostickValue-1)*3+2]=magn[2];
//	
//	if(KeyAndJostickValue>=3)
//	{
//			gstMagOffset.s16X = (TestmagnBuff[0]+TestmagnBuff[3])/2;
//			gstMagOffset.s16Y = (TestmagnBuff[1]+TestmagnBuff[4])/2;
//			gstMagOffset.s16Z = (TestmagnBuff[5]+TestmagnBuff[8])/2;
//			Flag_Check_Magn=true;
//	}
}
/******************* (C) COPYRIGHT 2014 Waveshare *****END OF FILE*******************/
