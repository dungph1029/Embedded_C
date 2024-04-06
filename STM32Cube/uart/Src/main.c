 /* File name: transmit and receive data with USART
 *
 * Description: USART6 -> USART1, LedGreen
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
//---------------------------------------------------------------------------
#define GPIO_PIN_SET 						1
#define GPIO_PIN_RESET 						0
#define GPIO_PIN_LOW 						0
#define GPIO_PIN_HIGH 						1

//---------------------------------------------------------------------------
#define GPIO_PIN_LED                 		GPIO_Pin_5
#define GPIO_PIN_BUTTON					 	GPIO_Pin_13

//USART----------------------------------------------------------------------
#define USART6_TX					         GPIO_Pin_6
#define USART6_GPIO					         GPIOC
#define USART6_GPIO_CLOCK					 RCC_AHB1Periph_GPIOC
#define USART6_CLOCK    					 RCC_APB2Periph_USART6


#define USART1_RX					         GPIO_Pin_10
#define USART1_GPIO					         GPIOA
#define USART1_GPIO_CLOCK					 RCC_AHB1Periph_GPIOA
#define USART1_CLOCK						 RCC_APB2Periph_USART1

#define USARTx_Baud					         9600

#define Check_Data                           0x10
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static void AppInitCommon(void);
static void Button_Init(void);
static void Led_Init(void);
static void USART1_Init(void);
static void USART6_Init(void);
static void Control_led_ReceiveData(void);
static void Delay_ms(uint32_t ms);
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/
//---------------------------------------------------------------------------
uint8_t Data_Receive;
//---------------------------------------------------------------------------
int main(void)
{
	AppInitCommon();

	while(1)
	{
		Control_led_ReceiveData();
	}
}

/**
 * @func   Initializes
 *
 * @brief  Initializes All Periperal
 *
 * @param  [None]
 *
 * @return value : None
 */
static void AppInitCommon(void)
{

	// System clock 84MHz------------------------------------------------------
	SystemCoreClockUpdate();

	// Initializes Button------------------------------------------------------
	Button_Init();

	// Initializes Led---------------------------------------------------------
	Led_Init();

	// Initializes USART1------------------------------------------------------
	USART1_Init();

	// Initializes USART6------------------------------------------------------
	USART6_Init();
}


/**
 * @func   Control Status Led When Receive Data
 *
 * @brief  Control_led_ReceiveData
 *
 * @param  [None]
 *
 * @return value : None
 */
static void Control_led_ReceiveData(void)
{
	//Send data from USART6 to USART1-----------------------------------------
    if (GPIO_ReadInputDataBit(GPIOC, GPIO_PIN_BUTTON) == 0)
    {
    	USART_SendData(USART6, 0x10);
    }

    //Check data--------------------------------------------------------------
    if (Data_Receive == Check_Data)
    {
    	for (int i = 0; i<5; i++)
    	{
    		GPIO_SetBits (GPIOA, GPIO_PIN_LED);
    		Delay_ms(1000);
    		GPIO_ResetBits (GPIOA, GPIO_PIN_LED);
    		Delay_ms(1000);
    	}

    	Data_Receive = 0;
    }
    GPIO_ResetBits (GPIOA, GPIO_PIN_LED);
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

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_BUTTON;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
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
	uint32_t i,j;
	for (i = 0 ; i < ms ; i ++)
	{
		for (j = 0; j<5000; j++){;}
	}
}

/**
 * @func   Initializes Use USART6 Periperal
 *
 * @brief  USART6_Init
 *
 * @param  [None]
 *
 * @return value : None
 */
static void USART6_Init(void)
{
	GPIO_InitTypeDef     GPIO_InitStructure;
	USART_InitTypeDef    USART_InitStructure;

	//Enable Clock USART1 And GPIOC------------------------------------------
	RCC_AHB1PeriphClockCmd(USART6_GPIO_CLOCK, ENABLE);

	//Initializes GPIO Use USART6 Periperal----------------------------------
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = USART6_TX;
	GPIO_Init(USART6_GPIO, &GPIO_InitStructure);

	//Initializes GPIO As Alternate function mode----------------------------
	GPIO_PinAFConfig(USART6_GPIO, GPIO_PinSource6, GPIO_AF_USART6);

	//Initializes USART6 Periperal-------------------------------------------
	RCC_APB2PeriphClockCmd(USART6_CLOCK,ENABLE);

	USART_InitStructure.USART_BaudRate = USARTx_Baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;

	USART_Init(USART6, &USART_InitStructure);

	USART_Cmd(USART6, ENABLE);

}

/**
 * @func   Initializes Use USART1 Periperal
 *
 * @brief  USART1_Init
 *
 * @param  [None]
 *
 * @return value : None
 */
static void USART1_Init(void)
{
    GPIO_InitTypeDef     GPIO_InitStructure;
    USART_InitTypeDef    USART_InitStructure;
    NVIC_InitTypeDef     NVIC_InitStructure;

    //Enable clock GPIO And USART1------------------------------------------
    RCC_AHB1PeriphClockCmd(USART1_GPIO_CLOCK, ENABLE);

    //Initializes GPIO Use USART1 Periperal---------------------------------
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

    GPIO_InitStructure.GPIO_Pin = USART1_RX ;
    GPIO_Init(USART1_GPIO, &GPIO_InitStructure);


    //Initializes GPIO As Alternate function mode---------------------------
	GPIO_PinAFConfig(USART1_GPIO, GPIO_PinSource10, GPIO_AF_USART1);

	//Initializes USART1 Periperal------------------------------------------
	RCC_APB2PeriphClockCmd(USART1_CLOCK, ENABLE);

    USART_InitStructure.USART_BaudRate = USARTx_Baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);

	//Enable USART----------------------------------------------------------
    USART_Cmd(USART1, ENABLE);

    //Enable USARTx Receive interrupts--------------------------------------
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	//NVIC configuration----------------------------------------------------
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	//Enable the USARTx Interrupt-------------------------------------------
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @func   Interuppt When Receive Data
 *
 * @brief  USART1_IRQHandler
 *
 * @param  [None]
 *
 * @return value : None
 */
void USART1_IRQHandler(void)
{
	 if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	 {
		 Data_Receive = USART_ReceiveData(USART1);
	 }
	 USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

