 /* File name: transmit and receive data with SPI
 *
 * Description: SPI Master -> SPI Slave, LedGreen
 *
 *
 * Last Changed By:  $Author: dungph1029 $
 * Revision:         $Revision: $
 * Last Changed:     $Date: $April 06, 2024
 *
 * Code sample:
 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include "stm32f401re_rcc.h"
#include "stm32f401re_gpio.h"
#include "stm32f401re_spi.h"
#include "misc.h"
/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/
//-----------------------------------------------------------------------------
#define GPIO_PIN_SET 						 1
#define GPIO_PIN_RESET 						 0
#define GPIO_PIN_LOW 						 0
#define GPIO_PIN_HIGH 						 1

//-----------------------------------------------------------------------------
#define GPIO_PIN_LED                         GPIO_Pin_5
#define Button								 GPIO_Pin_13
#define Pin_Button							 GPIO_Pin_13
#define Pin_NSS							     GPIO_Pin_12

#define SPI_Master_GPIO_Clock             	 RCC_AHB1Periph_GPIOB
#define SPI_Master_Clock					 RCC_APB1Periph_SPI2
#define SPI_Master_GPIO						 GPIOB
#define SPI_Master							 SPI2
#define NSS_Master							 GPIO_Pin_12
#define SCK_Master 							 GPIO_Pin_13
#define MISO_Master 						 GPIO_Pin_14
#define MOSI_Master 						 GPIO_Pin_15

#define SPI_Salve_GPIO_Clock        		 RCC_AHB1Periph_GPIOA
#define SPI_Salve_Clock				         RCC_APB2Periph_SPI1
#define SPI_Salve_GPIO						 GPIOA
#define SPI_Slave							 SPI1
#define NSS_Slave							 GPIO_Pin_4
#define SCK_Slave 							 GPIO_Pin_3
#define MISO_Slave 				    		 GPIO_Pin_6
#define MOSI_Slave 				   			 GPIO_Pin_7

#define Check_DataSlave					     0xB1
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void AppInitCommon(void);
static void SPI_InitMaster(void);
static void SPI_InitSlave(void);
static void Led_Init(void);
static void Button_Init(void);
static void Delay(uint32_t ms);
static uint8_t Revice_Data_Slave(SPI_TypeDef * SPIx);
static void Send_Data(SPI_TypeDef * SPIx, uint8_t data);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
uint16_t Recive_Data;
/******************************************************************************/
/**
 * @func   Initializes SPI Master
 *
 * @brief  SPI_InitMaster
 *
 * @param  [None]
 *
 * @return value : None
 *
 */
static void SPI_InitMaster(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	SPI_InitTypeDef     SPI_InitStructure;
	NVIC_InitTypeDef    NVIC_InitStructure;

	//Connect Clock to GPIOB---------------------------------------------------
	RCC_AHB1PeriphClockCmd(SPI_Master_GPIO_Clock, ENABLE);

	//Initialization GPIO Use For SPI------------------------------------------
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_Pin = SCK_Master | MISO_Master | MOSI_Master;
	GPIO_Init(SPI_Master_GPIO, &GPIO_InitStructure);

	//Connect SPI1 pins to SPI Alternate Function------------------------------
	GPIO_PinAFConfig(SPI_Master_GPIO, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(SPI_Master_GPIO, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(SPI_Master_GPIO, GPIO_PinSource15, GPIO_AF_SPI2);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

	GPIO_InitStructure.GPIO_Pin = NSS_Master;
	GPIO_Init(SPI_Master_GPIO, &GPIO_InitStructure);

	//Enable peripheral clock--------------------------------------------------
	RCC_APB1PeriphClockCmd(SPI_Master_Clock, ENABLE);

	//Set to full duplex mode, seperate MOSI and MISO lines--------------------
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;

	//Transmit in master mode--------------------------------------------------
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

	//One packet of data is 8 bits wide----------------------------------------
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;

	//Clock is low when idle---------------------------------------------------
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;

	//Data sampled at first edge-----------------------------------------------
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;

	//Set NSS us software------------------------------------------------------
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft ;

	//SPI frequency is APB2 frequency / 4-------------------------------------
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

	//Data is transmitted LSB first--------------------------------------------
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;

	SPI_Init(SPI_Master, &SPI_InitStructure);

	//Enable SPI2--------------------------------------------------------------
	SPI_Cmd(SPI_Master, ENABLE);

}
/**
 * @func   Initializes SPI Slave
 *
 * @brief  SPI_InitSlave
 *
 * @param [None]
 *
 * @return value : None
 */
static void SPI_InitSlave(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	SPI_InitTypeDef     SPI_InitStructure;
	NVIC_InitTypeDef    NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(SPI_Salve_GPIO_Clock, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = MISO_Slave | MOSI_Slave;
	GPIO_Init(SPI_Salve_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = SCK_Slave;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(SPI_Salve_GPIO, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(SPI_Salve_GPIO, GPIO_PinSource7, GPIO_AF_SPI1);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

	GPIO_InitStructure.GPIO_Pin = NSS_Slave;
	GPIO_Init(SPI_Salve_GPIO, &GPIO_InitStructure);

	//enable peripheral clock SPI1---------------------------------------------
	RCC_APB2PeriphClockCmd(SPI_Salve_Clock, ENABLE);

	// set to full duplex mode, seperate MOSI and MISO lines-------------------
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;

	//Use SPI1 as slave mode---------------------------------------------------
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;

	//One packet of data is 8 bits wide----------------------------------------
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;

	//Clock is low when idle---------------------------------------------------
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;

	//Data sampled at first edge-----------------------------------------------
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;

	//SPI frequency is APB2 frequency / 4--------------------------------------
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;

	//Set NSS us software------------------------------------------------------
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;

	//data is transmitted MSB first--------------------------------------------
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;


	SPI_Init(SPI_Slave, &SPI_InitStructure);

	//Enable SPI1--------------------------------------------------------------
	SPI_Cmd(SPI_Slave, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	SPI_ITConfig(SPI_Slave, SPI_I2S_IT_RXNE, ENABLE);

}
/**
 * @func   Send Data From Master
 *
 * @brief  Send_Data
 *
 * @param  [None]
 *
 * @return value : None
 */
static void Send_Data(SPI_TypeDef * SPIx, uint8_t data)
{
	//Allow Send Data to Slave--------------------------------------------------
	GPIO_ResetBits(SPI_Master_GPIO, Pin_NSS);

	SPI_I2S_SendData(SPIx,data);

	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET){;}

	//Allow Send Data to Slave--------------------------------------------------
	GPIO_SetBits (SPI_Master_GPIO, Pin_NSS);
}
/**
 * @func   Recive Data From Master
 *
 * @brief  Revice_Data
 *
 * @param  [None]
 *
 * @return value : None
 */
static uint8_t Revice_Data_Slave(SPI_TypeDef * SPIx)
{
	uint8_t Data_Revice = 0;

	Data_Revice = SPI_I2S_ReceiveData(SPIx);

	return Data_Revice;
}
/**
 * @func   The Function Executes The Interrupt
 *
 * @brief  SPI1_IRQHandler
 *
 * @param  [None]
 * @return value : None
 */
void SPI1_IRQHandler(void)
{

	if(SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET)
	{
		Recive_Data = Revice_Data_Slave(SPI1);
	}

	SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);
}
/**
 * @func   Initializes Use Led
 *
 * @brief  Led_Init
 *
 * @param  [None]
 *
 * @return value : None
 */
static void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @func   Initializes GPIO Use Button
 *
 * @brief  Button_Init
 *
 * @param  [None]
 *
 * @return value : None
 */
static void Button_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure ;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

	GPIO_InitStructure.GPIO_Pin = Button;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//------------------------------------------------------------------------------
int main(void)
{

	AppInitCommon();

	while(1)
	{
		//Send data when Button pressed-----------------------------------------
		if(GPIO_ReadInputDataBit(GPIOC, Pin_Button) == GPIO_PIN_LOW)
		{
	    	Delay(20);
			Send_Data(SPI2, 0xB1);

		}
		//Turn on led when data = 0x20------------------------------------------
		if (Recive_Data == Check_DataSlave)
		{

		    for (int i = 0; i<8; i++)
		    {
		    	GPIO_SetBits(GPIOA, GPIO_PIN_LED);
		    	Delay(1000);
		    	GPIO_ResetBits(GPIOA, GPIO_PIN_LED);
		    	Delay(1000);
		    }
		    Recive_Data = 0;
		}
		GPIO_ResetBits(GPIOA, GPIO_PIN_LED);
	}
}
/**
 * @func   Initializes All Periperal
 *
 * @brief  AppInitCommon
 *
 * @param  [None]
 *
 * @return value : None
 */
static void AppInitCommon(void)
{
	//System_Clock--------------------------------------------------------------
	SystemCoreClockUpdate();

	//Button_Init---------------------------------------------------------------
	Button_Init();

	//Led Init------------------------------------------------------------------
	Led_Init();

	//SPI_Master_Init-----------------------------------------------------------
	SPI_InitMaster();

	//SPI_Slave_Init------------------------------------------------------------
	SPI_InitSlave();

}
/**
 * @func   Delay Time
 * @brief  Delay
 *
 * @param  [None]
 *
 * @return value : None
 */
static void Delay(uint32_t ms)
{
	uint32_t i,j;
	for (i = 0 ; i < ms ; i ++)
	{
		for (j = 0; j<5000; j++){;}
	}
}

