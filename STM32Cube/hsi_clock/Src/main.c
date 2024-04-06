 /* File name: Configure clock supplies
 *
 * Description: Configure output on OneNode MCU is HSI
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
#include <stdint.h>
/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/
#define RCC_BASE_ADDR              0x40023800UL
#define RCC_CFGR_REG_OFFSET        0x08UL

#define RCC_CR_REG_OFFSET          0x00UL
#define RCC_CFGR_REG_ADDR          (RCC_BASE_ADDR + RCC_CFGR_REG_OFFSET)

#define RCC_CR_REG_ADDR            (RCC_BASE_ADDR + RCC_CR_REG_OFFSET )
#define GPIOA_BASE_ADDR            0x40020000UL
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

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/

int main(void)
{
	uint32_t *pRccCrReg = (uint32_t*)RCC_CR_REG_ADDR;
	uint32_t *pRccCfgrReg =  (uint32_t*) RCC_CFGR_REG_ADDR;

	*pRccCrReg |= ( 1 << 16);
	while( ! (*pRccCrReg & ( 1 << 17) ) );
	*pRccCfgrReg |= ( 1 << 0);
	*pRccCfgrReg |= ( 1 << 22); //clear 21 and SET 22
	*pRccCfgrReg |= ( 1 << 25);
	*pRccCfgrReg |= ( 1 << 26);

	uint32_t *pRCCAhb1Enr = (uint32_t*)(RCC_BASE_ADDR + 0x30);
	*pRCCAhb1Enr |= ( 1 << 0); // Enable GPIOA peripheral clock

	uint32_t *pGPIOAModeReg = (uint32_t*)(GPIOA_BASE_ADDR + 00);
	*pGPIOAModeReg &= ~( 0x3 << 16); //clear
	*pGPIOAModeReg |= ( 0x2 << 16);  //set

	uint32_t *pGPIOAAltFunHighReg = (uint32_t*)(GPIOA_BASE_ADDR + 0x24);
	*pGPIOAAltFunHighReg &= ~( 0x0f << 0);

}
