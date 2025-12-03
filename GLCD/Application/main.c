
/********************************************************************************************************
*  [FILE NAME]   :      <main.c>                                                                        *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <Dec 5, 2024>                                                                   *
*  [Description] :      <Source file for the AVR GLCD Display Main application driver>                  *
********************************************************************************************************/

#include "AppConfig.h"
#include "../HAL/Inc/Images.h"
LCD_t LCD1 =
{
	.LCD_RS.PORTx      = PORTA_INDEX,
	.LCD_RS.PINx       = PIN2_INDEX,
	
	.LCD_EN.PORTx      = PORTD_INDEX,
	.LCD_EN.PINx       = PIN6_INDEX,
	
	.LCD_Data[0].PORTx = PORTC_INDEX,
	.LCD_Data[0].PINx  = PIN4_INDEX,
	
	.LCD_Data[1].PORTx = PORTC_INDEX,
	.LCD_Data[1].PINx  = PIN5_INDEX,
	
	.LCD_Data[2].PORTx = PORTC_INDEX,
	.LCD_Data[2].PINx  = PIN6_INDEX,
	
	.LCD_Data[3].PORTx = PORTC_INDEX,
	.LCD_Data[3].PINx  = PIN7_INDEX,
	
	.LCD_Bits          = LCD4Bit
};

GLCD_Config_t GLCD1 =
{
	.GLCD_RS_PIN.PORTx = PORTD_INDEX,
	.GLCD_RS_PIN.PINx  = PIN7_INDEX,
	
	.GLCD_RW_PIN.PORTx = PORTC_INDEX,
	.GLCD_RW_PIN.PINx  = PIN7_INDEX,
	
	.GLCD_EN_PIN.PORTx = PORTC_INDEX,
	.GLCD_EN_PIN.PINx  = PIN6_INDEX,
	
	.GLCD_CS1_PIN.PORTx = PORTA_INDEX,
	.GLCD_CS1_PIN.PINx  = PIN2_INDEX,
	
	.GLCD_CS2_PIN.PORTx = PORTB_INDEX,
	.GLCD_CS2_PIN.PINx  = PIN1_INDEX,
	
	.GLCD_RST_PIN.PORTx = PORTB_INDEX,
	.GLCD_RST_PIN.PINx  = PIN0_INDEX,
	
	.GLCD_DATA_PINS[0].PORTx = PORTC_INDEX,
	.GLCD_DATA_PINS[0].PINx  = PIN5_INDEX,
	
	.GLCD_DATA_PINS[1].PORTx = PORTC_INDEX,
	.GLCD_DATA_PINS[1].PINx  = PIN4_INDEX,
	
	.GLCD_DATA_PINS[2].PORTx = PORTC_INDEX,
	.GLCD_DATA_PINS[2].PINx  = PIN3_INDEX,
	
	.GLCD_DATA_PINS[3].PORTx = PORTC_INDEX,
	.GLCD_DATA_PINS[3].PINx  = PIN2_INDEX,
	
	.GLCD_DATA_PINS[4].PORTx = PORTC_INDEX,
	.GLCD_DATA_PINS[4].PINx  = PIN1_INDEX,
	
	.GLCD_DATA_PINS[5].PORTx = PORTC_INDEX,
	.GLCD_DATA_PINS[5].PINx  = PIN0_INDEX,
	
	.GLCD_DATA_PINS[6].PORTx = PORTD_INDEX,
	.GLCD_DATA_PINS[6].PINx  = PIN6_INDEX,
	
	.GLCD_DATA_PINS[7].PORTx = PORTA_INDEX,
	.GLCD_DATA_PINS[7].PINx  = PIN3_INDEX
};

const uint8 SmilecustomChar[] =
{
	0x00,
	0x0A,
	0x00,
	0x04,
	0x11,
	0x0E,
	0x00,
	0x00
};

const uint8 HeartcustomChar[] =
{
	0x00,
	0x0A,
	0x1F,
	0x1F,
	0x0E,
	0x04,
	0x00,
	0x00
};

int main(void)
{
	GLCD_Init(&GLCD1);
	GLCD_Clear(&GLCD1);
	
	/* Display a string on the display */
	GLCD_WriteString(&GLCD1, 3, "Welcome David");
	_delay_ms(2000);
	/* Clear the display */
	GLCD_Clear(&GLCD1);
	
	while(1)
	{
		/* Display an image on the display */
		GLCD_DrawBitmap(&GLCD1, img);
		_delay_ms(500);
		GLCD_DrawBitmap(&GLCD1, img1);
		_delay_ms(500);
	}
	return 0;
}

