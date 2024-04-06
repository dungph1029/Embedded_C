 /* File name: Configure Timer
 *
 * Description: Create Delay Time
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
#include "stm32f401re_tim.h"
#include "misc.h"
/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/
#define LED_GREEN_PORT						GPIOA
#define LED_GREEN_PIN						GPIO_PIN_5
#define LED_GREEN							5

#define GPIO_PIN_SET						1
#define GPIO_PIN_RESET						0
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/


/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static void Led1Control_Init(void);
static void BuzzerControl_SetStatus(GPIO_TypeDef * GPIOx, uint8_t GPIO_PIN, uint8_t Status);
static void Time_Init(void);
static void Delay_ms(uint32_t ms);
static void AppInitCommon(void);
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
/**
 * @func   Initialize LED Control
 *
 * @brief  Led1Control_Init
 *
 * @param  [None]
 *
 * @return value : None
 */
static void Led1Control_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;

	GPIO_Init(LED_GREEN_PORT, &GPIO_InitStructure);
}
/**
 * @func   Led When Receive Data
 *
 * @brief  LedControl_SetState
 *
 * @param  [None]
 *
 * @return value : None
 */
static void BuzzerControl_SetStatus(GPIO_TypeDef * GPIOx, uint8_t GPIO_PIN, uint8_t Status)
{

	if(Status == GPIO_PIN_SET)
	{
		//Set bin in BSRR Registers---------------------------------------------------------
		GPIOx->BSRRL |= 1 << GPIO_PIN;
	}
	else
	{
		//Reset bit in BSRR Registers--------------------------------------------------------
		GPIOx->BSRRH |= 1 << GPIO_PIN;
	}
}
/**
 * @func   Timer
 *
 * @brief  Timer_Init
 *
 * @param  [None]
 *
 * @return value : None
 */
static void Time_Init(void)
{
	TIM_TimeBaseInitTypeDef Timer_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	Timer_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	Timer_InitStructure.TIM_Prescaler = 83;
	Timer_InitStructure.TIM_Period = 999;
	Timer_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	Timer_InitStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &Timer_InitStructure);
	TIM_Cmd(TIM1, ENABLE);
}
/**
 * @func   Delay Timer
 * @brief  Delay_ms
 *
 * @param  [None]
 *
 * @return value : None
 */
static void Delay_ms(uint32_t ms)
{
	while(ms != 0)
	{
		TIM_SetCounter(TIM1, 0);
		while(TIM_GetCounter(TIM1) != 999);
		ms--;
	}
}
static void AppInitCommon(void)
{

	// System clock 84MHz------------------------------------------------------
	SystemCoreClockUpdate();

	// Initializes Time--------------------------------------------------------
	Time_Init();

	// Initializes Led1Control-------------------------------------------------
	Led1Control_Init();

}
//------------------------------------------------------------------------------
uint8_t Data_Receive;
//------------------------------------------------------------------------------
int main(void)
{
	AppInitCommon();
	while(1)
	{
		LedControl_SetState(LED_GREEN_PORT, LED_GREEN, GPIO_PIN_SET);
		Delay_ms(500);
		LedControl_SetState(LED_GREEN_PORT, LED_GREEN, GPIO_PIN_RESET);
		Delay_ms(500);
	}
}

