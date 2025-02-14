 /* File name: transmit and receive data with I2C
 *
 * Description: I2C1, I2C3 and Blink LED
 *
 *
 * Last Changed By:  $Author: dungph1029$
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
#define SERIAL_ADR                           0x02
#define DATA_RCV_VALID                       0x10
#define DATA_RCV_IDLE                        0x00

#define I2C_SPEED							 400000 // 400 KBit/s

#define LED_NUM_OF_BLINK                     5

#define LED_IND_GPIO_RCC                     RCC_AHB1Periph_GPIOA
#define LED_IND_PIN                        	 GPIO_Pin_5
#define LED_IND_PORT                         GPIOA

#define BUTTON_CFG_GPIO_RCC                  RCC_AHB1Periph_GPIOC
#define BUTTON_CFG_PIN					     GPIO_Pin_13
#define BUTTON_CFG_PORT				         GPIOC

#define I2C_SLAVE_RCC						 RCC_APB1Periph_I2C1
#define I2C_SLAVE_INSTANCE					 I2C1

#define I2C_SLAVE_GPIO_RCC		    		 RCC_AHB1Periph_GPIOB
#define I2C_SLAVE_PORT						 GPIOB
#define SDA_SLAVE_PIN			    		 GPIO_Pin_9
#define SCL_SLAVE_PIN			    		 GPIO_Pin_8

#define I2C_MASTER_RCC						 RCC_APB1Periph_I2C3
#define I2C_MASTER_INSTANCE					 I2C3

#define I2C_MASTER_GPIO_RCC		    		 RCC_AHB1Periph_GPIOA
#define I2C_MASTER_PORT						 GPIOA
#define SDA_MASTER_PIN			    		 GPIO_Pin_9 // PC9
#define SCL_MASTER_PIN			    		 GPIO_Pin_8 // PA8
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
static void Led_Init(void);
static void Button_Init(void);
static void I2C_InitMaster(void);
static void I2C_InitSlave(void);
static void I2C_Start(void);
static void I2C_SendAddress(uint8_t address);
static void I2C_TransmitData(uint8_t data);
static void I2C_Stop(void);
static void Delay_ms(uint32_t ms);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/

uint8_t Data_Receive;

int main(void)
{
	AppInitCommon();

	while(1)
	{
	    if (GPIO_ReadInputDataBit(BUTTON_CFG_PORT, BUTTON_CFG_PIN) == Bit_RESET)
		{
			I2C_Start();

			I2C_SendAddress(SERIAL_ADR);

			I2C_TransmitData(DATA_RCV_VALID);

			I2C_Stop();
		}

	    if (Data_Receive == DATA_RCV_VALID)
	    {
	    	for (int i = 0; i < LED_NUM_OF_BLINK; i++)
	    	{
	    		GPIO_SetBits(LED_IND_PORT, LED_IND_PIN);
	    		Delay_ms(1000);
	    		GPIO_ResetBits(LED_IND_PORT, LED_IND_PIN);
	    		Delay_ms(1000);
	    	}
	    	Data_Receive = DATA_RCV_IDLE;
	    }
	}
}

/**
 * @func   Initializes
 *
 * @brief  Initializes All Peripheral
 *
 * @param  [None]
 *
 * @return value : None
 */
static void AppInitCommon(void)
{
	// System clock 84MHz ------------------------------------------------------
	SystemCoreClockUpdate();

	// Initializes Button User -------------------------------------------------
	Button_Init();

	// Initializes Led ---------------------------------------------------------
	Led_Init();

	// Initializes I2C3 Master -------------------------------------------------
	I2C_InitMaster();

	// Initializes I2C1 Slave --------------------------------------------------
	I2C_InitSlave();
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

	RCC_AHB1PeriphClockCmd(LED_IND_GPIO_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_IND_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(LED_IND_PORT, &GPIO_InitStructure);
}

/**
 * @func   Initializes Use Button
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

	RCC_AHB1PeriphClockCmd(BUTTON_CFG_GPIO_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = BUTTON_CFG_PIN;
	GPIO_Init(BUTTON_CFG_PORT, &GPIO_InitStructure);
}

/**
 * @func   Initializes Use I2C Slave
 *
 * @brief  I2C_InitSlave
 *
 * @param  [None]
 *
 * @return value : None
 */
static void I2C_InitSlave(void)
{
	// Initialization structure ------------------------------------------------
	I2C_InitTypeDef I2C_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	// Initialize GPIO as open drain alternate function ------------------------
	RCC_APB1PeriphClockCmd(I2C_SLAVE_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(I2C_SLAVE_GPIO_RCC, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_InitStruct.GPIO_Pin = SCL_SLAVE_PIN | SDA_SLAVE_PIN;
	GPIO_Init(I2C_SLAVE_PORT, &GPIO_InitStruct);

	// Connect PB8 to I2C1_SCL -------------------------------------------------
	GPIO_PinAFConfig(I2C_SLAVE_PORT, GPIO_PinSource8, GPIO_AF_I2C1);

	// Connect PB9 to I2C1_SDA -------------------------------------------------
	GPIO_PinAFConfig(I2C_SLAVE_PORT, GPIO_PinSource9, GPIO_AF_I2C1);

	// Initialize I2C1 ---------------------------------------------------------
	I2C_InitStruct.I2C_ClockSpeed = I2C_SPEED;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = SERIAL_ADR;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

	I2C_Init(I2C_SLAVE_INSTANCE, &I2C_InitStruct);
	I2C_Cmd(I2C_SLAVE_INSTANCE, ENABLE);

	// Configure interrupt -----------------------------------------------------
	I2C_ITConfig(I2C_SLAVE_INSTANCE, I2C_IT_EVT, ENABLE);
	I2C_ITConfig(I2C_SLAVE_INSTANCE, I2C_IT_BUF, ENABLE);

	NVIC_InitStruct.NVIC_IRQChannel = I2C1_EV_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStruct);
}

/**
 * @func   Initializes Use I2C Master
 *
 * @brief  I2C_InitMaster
 *
 * @param  [None]
 *
 * @return value : None
 */
static void I2C_InitMaster(void)
{
	// Initialization structure ------------------------------------------------
	I2C_InitTypeDef I2C_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	// Initialize GPIO as open drain alternate function ------------------------
	//Enable the i2c
	RCC_APB1PeriphClockCmd(I2C_MASTER_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(I2C_MASTER_GPIO_RCC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

	GPIO_InitStruct.GPIO_Pin = SCL_MASTER_PIN;
	GPIO_Init(I2C_MASTER_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = SDA_MASTER_PIN;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	// Connect PA8 to I2C3_SCL -------------------------------------------------
	GPIO_PinAFConfig(I2C_MASTER_PORT, GPIO_PinSource8, GPIO_AF_I2C3);

	// Connect PB4 to I2C3_SDA -------------------------------------------------
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3);

	// Initialize I2C3 ---------------------------------------------------------
	I2C_InitStruct.I2C_ClockSpeed = I2C_SPEED;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

	I2C_Init(I2C_MASTER_INSTANCE, &I2C_InitStruct);
	I2C_Cmd(I2C_MASTER_INSTANCE, ENABLE);
}

/**
 * @func   Send bit Start And Address
 *
 * @brief  I2C_StartAddress
 *
 * @param  [None]
 *
 * @return value : None
 */
static void I2C_Start(void)
{
	// Wait until I2Cx is not busy anymore
	while (I2C_GetFlagStatus(I2C_MASTER_INSTANCE, I2C_FLAG_BUSY));

	// Generate start condition
	I2C_GenerateSTART(I2C_MASTER_INSTANCE, ENABLE);

	// Wait for I2C EV5.
	// It means that the start condition has been correctly released
	// on the I2C bus (the bus is free, no other devices is communicating))
	while (!I2C_CheckEvent(I2C_MASTER_INSTANCE, I2C_EVENT_MASTER_MODE_SELECT));
}

static void I2C_SendAddress(uint8_t address) {
	I2C_Send7bitAddress(I2C_MASTER_INSTANCE, address, I2C_Direction_Transmitter);

	while (!I2C_CheckEvent(I2C_MASTER_INSTANCE, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
}

/**
 * @func   I2C_TransmitData
 *
 * @brief  Send bits data
 *
 * @param  [data]
 *
 * @return value : None
 */
static void I2C_TransmitData(uint8_t data)
{
	// Send data byte
	I2C_SendData(I2C_MASTER_INSTANCE, data);
	// Wait for I2C EV8_2.
	// It means that the data has been physically shifted out and
	// output on the bus)
	while (!I2C_CheckEvent(I2C_MASTER_INSTANCE, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

/**
 * @func   Send bit Stop
 *
 * @brief  I2C_Stop
 *
 * @param  [None]
 *
 * @return value : None
 */
static void I2C_Stop(void)
{
	// Generate I2C stop condition
	I2C_GenerateSTOP(I2C_MASTER_INSTANCE, ENABLE);
}

/**
 * @func   Interrupt When Receive Data
 *
 * @brief  I2C1_EV_IRQHandler
 *
 * @param  [None]
 * @return value : None
 */
void I2C1_EV_IRQHandler(void)
{
	switch (I2C_GetLastEvent(I2C_SLAVE_INSTANCE))
	{
	    case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:
	    	// The address sent by the master matches the own address of the peripheral
	        I2C_ClearFlag(I2C_SLAVE_INSTANCE, I2C_FLAG_ADDR);
	        break;

	    case I2C_EVENT_SLAVE_BYTE_RECEIVED:
	    	Data_Receive = I2C_ReceiveData(I2C_SLAVE_INSTANCE);
	        I2C_ClearFlag(I2C_SLAVE_INSTANCE, I2C_FLAG_RXNE);
	        break;

	    case I2C_EVENT_SLAVE_STOP_DETECTED:
	    	// Disable bit stop I2C1
	    	I2C_AcknowledgeConfig(I2C_SLAVE_INSTANCE, ENABLE);
            break;

	    default:
		    break;
    }

    I2C_ClearITPendingBit(I2C_SLAVE_INSTANCE, I2C_IT_RXNE);
}

/**
 * @func   Delay Time
 *
 * @brief  Delay
 *
 * @param  [None]
 *
 * @return value : None
 */
static void Delay_ms(uint32_t ms)
{
	uint32_t i, j;

	for (i = 0 ; i < ms ; i ++)
	{
		for (j = 0; j < 5000; j++) {;}
	}
}

