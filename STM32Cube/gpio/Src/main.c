 /* File name: configure input, output status GPIO
 *
 * Description: led green on, off and alarm
 *
 *
 * Last Changed By:  $Author: dungph1029 $
 * Revision:         $Revision: $
 * Last Changed:     $Date: $April 05, 2024
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
#define BUTTON_PRESS						0
#define BUTTON_RELEASE						1

//Define LED
#define LED_GPIO_PORT						GPIOA
#define LED_GPIO_PIN						GPIO_Pin_0
#define LED_GPIO_CLOCK_EN					RCC_AHB1Periph_GPIOA
//Define Button
#define BUZZER_GPIO_PORT					GPIOC
#define BUZZER_GPIO_PIN						GPIO_Pin_9
#define BUZZER_GPIO_CLOCK_EN				RCC_AHB1Periph_GPIOC
//Define Button
#define BUZZER_GPIO_PORT					GPIOB
#define BUZZER_GPIO_PIN						GPIO_Pin_3
#define BUZZER_GPIO_CLOCK_EN				RCC_AHB1Periph_GPIOB
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
static void mainInit(void);
static void led_Init(void);
static void buttonB2_Init(void);
static void buzzer_Init(void);
static void Delay(uint32_t ms);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/

/**
 * @brief Initialize LED GPIO
 *
 * @param [None]
 *
 * @return value : None
 */
static void led_Init(void)
{
	// Bước 1: Khai báo GPIO kiểu structure
	GPIO_InitTypeDef GPIO_InitStructure;

	// Bước 2: Cấp xung clock hoạt động cho Port A.
	RCC_AHB1PeriphClockCmd(LED_GPIO_CLOCK_EN, ENABLE);

	// Bước 3: Cấu hình các thông số cho GPIO: Pin, Mode, Speed, OType, Pu/Pd.
	GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// Bước 4: Khởi tạo các giá trị sử dụng hàm GPIO_Init.
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
}
/**
 * @brief Initialize Buzzer GPIO
 *
 * @param [None]
 *
 * @return value : None
 */
static void buzzer_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(BUZZER_GPIO_CLOCK_EN, ENABLE);

	GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);
}
/**
 * @brief Initialize Button B2
 *
 * @param [None]
 *
 * @return value : None
 */
static void buttonB2_Init(void)
{
	// Bước 1: Khai báo GPIO kiểu structure
	GPIO_InitTypeDef GPIO_InitStructure;

	// Bước 2: Cấp xung clock hoạt động cho Port B.
	RCC_AHB1PeriphClockCmd(BUTTON_GPIO_CLOCK_EN, ENABLE);

	// Bước 3: Cấu hình các thông số cho GPIO: Pin, Mode, Pu/Pd.
	// Chọn chân
	GPIO_InitStructure.GPIO_Pin = BUTTON_GPIO_PIN;

	// Chọn Mode Input
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

	// Trạng thái ban đầu kéo lên
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// Khởi tạo các giá trị cho GPIO
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

int main(void)
{
    // Initialize
	mainInit();

	while(1)
	{
		if(GPIO_ReadInputDataBit(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN) == BUTTON_PRESS)
		{
			GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);				// Turn ON LED
			GPIO_SetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);		// Turn ON Buzzer
		}
		else
		{
			GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN);			// Turn OFF LED
			GPIO_ResetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);		// Turn OFF Buzzer
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
