 /* File name: configure input, output status GPIO
 *
 * Description: Invert the LedRed output status
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
#include <stdio.h>
#include <stdint.h>
#include <stm32f401re.h>
#include <stm32f401re_rcc.h>
#include <stm32f401re_gpio.h>
/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/
// Define Logic GPIO_PIN--------------------------------------------------------
#define GPIO_PIN_SET						1
#define GPIO_PIN_RESET						0
#define GPIO_PIN_LOW						0
#define GPIO_PIN_HIGH						1

// Define GPIO_BUZZER-----------------------------------------------------------
#define LEDRED_GPIO_PORT					GPIOB
#define LEDRED_GPIO_PIN						GPIO_Pin_13
#define LEDRED_PIN13						13
#define LEDREDControl_SetClock				RCC_AHB1Periph_GPIOC

// Define GPIO_BUTTON-----------------------------------------------------------
#define BUTTON_GPIO_PORT					GPIOB
#define BUTTON_GPIO_PIN						GPIO_Pin_3
#define BUTTON_B2_PIN						3
#define BUTTONControl_SetClock				RCC_AHB1Periph_GPIOB
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
static void Led_Init(void);
static void Button_Init(void);
static void LedControl_SetStatus(GPIO_TypeDef * GPIOx, uint8_t GPIO_PIN, uint8_t Status);
static uint8_t ButtonRead_Status(GPIO_TypeDef * GPIOx, uint32_t GPIO_PIN);
static void mainInit(void);
static void Delay(uint32_t ms);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/
/**
 * @brief Initialize LEDRED GPIO
 *
 * @param [None]
 *
 * @return value : None
 */
static void Led_Init(void)
{
	//Declare type variable GPIO Struc-------------------------------------------
	GPIO_InitTypeDef GPIO_InitStructure;

	//Enable Clock GPIOC---------------------------------------------------------
	RCC_AHB1PeriphClockCmd(LEDREDControl_SetClock, ENABLE);

	//Choose Pin Buzzer----------------------------------------------------------
	GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN;

	//Choose Pin Buzzer as Out---------------------------------------------------
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	//Choose Speed Pin-----------------------------------------------------------
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	//Select Type----------------------------------------------------------------
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	//Select status--------------------------------------------------------------
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

	//The function initializes all of the above values---------------------------
	GPIO_Init(LEDRED_GPIO_PORT, &GPIO_InitStructure);
}
/**
 * @brief Initialize Button
 *
 * @param [None]
 *
 * @return value : None
 */
static void Button_Init(void)
{
	//Declare type variable GPIO Struct-----------------------------------------
	GPIO_InitTypeDef GPIO_InitStructure;

	//Enable Clock GPIOA--------------------------------------------------------
	RCC_AHB1PeriphClockCmd(BUTTONControl_SetClock, ENABLE);

	//Choose Pin Led------------------------------------------------------------
	GPIO_InitStructure.GPIO_Pin = BUTTON_GPIO_PIN;

	//Choose Pin Led as Out-----------------------------------------------------
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	//Choose Speed Pin----------------------------------------------------------
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	//Select status-------------------------------------------------------------
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	//The function initializes all of the above values--------------------------
	GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);
}
/**
 * @brief Initialize Buzzer Control Set Status
 *
 * @param [None]
 *
 * @return value : None
 */
static void LedControl_SetStatus(GPIO_TypeDef * GPIOx, uint8_t GPIO_PIN, uint8_t Status)
{
	//Set bin in BSRR Registers---------------------------------------------------------
	if(Status == GPIO_PIN_SET)
	{
		GPIOx->BSRRL |= 1 << GPIO_PIN;
	}
	//Reset bit in BSRR Registers--------------------------------------------------------
	if(Status == GPIO_PIN_RESET)
	{
		GPIOx->BSRRH |= 1 << GPIO_PIN;
	}
}
/**
 * @brief Initialize Button Read Status
 *
 * @param [None]
 *
 * @return value : None
 */
static uint8_t ButtonRead_Status(GPIO_TypeDef * GPIOx, uint32_t GPIO_PIN)
{
	uint32_t Read_Pin;

	//Read bit in IDR Registers----------------------------------------------------------
	Read_Pin = (GPIOx->IDR) >> GPIO_PIN;
	Read_Pin - Read_Pin & 0x01;

	return Read_Pin;
}
int main(void)
{
    // Initialize
	mainInit();

	while(1)
	{
		if(GPIO_ReadInputDataBit(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN) == BUTTON_PRESS)
		{
			GPIO_SetBits(LEDRED_GPIO_PORT, BUZZER_GPIO_PIN);		// Turn ON LEDRED
		}
		else
		{
			GPIO_ResetBits(LEDRED_GPIO_PORT, BUZZER_GPIO_PIN);		// Turn OFF LEDRED
		}
	}
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
