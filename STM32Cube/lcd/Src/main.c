 /* File name: LCD screen
 *
 * Description: Displays text "I love Embedded Programming" on the LCD screen
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
#include <system_stm32f4xx.h>
#include <timer.h>
#include <Ucglib.h>
/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                         */
/******************************************************************************/


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

static ucg_t	ucg;

int main(void)
{
	SystemCoreClockUpdate();								// cấp clock
	TimerInit;												// để khởi tạo chức năng timer.

	// Khởi tạo LCD
	Ucglib4WireSWSPI_begin(&ucg, UCG_FONT_MODE_SOLID);
	ucg_ClearScreen(&ucg);

	ucg_SetFont(&ucg, ucg_font_helvR08_tf);					// lựa chọn font chữ và cỡ chữ
	ucg_SetColor(&ucg, 0, 255, 255, 255);					// chọn màu chữ là màu trắng
	ucg_SetColor(&ucg, 1, 0, 0, 0);							// màu background là màu đen
	ucg_SetRotate(&ucg);									// quay màn hình lại 180 độ
	ucg_DrawString(&ucg, 0, 12, 0, "I love Embedded");		// In dòng chữ ra LCD
	ucg_DrawString(&ucg, 0, 26, 0, "Programming");

	/* Loop forever */
	while(1) {
		processTimerScheduler();
	}
}
