
/********************************************************************************************************
 *  [FILE NAME]   :      <GLCD12864.h>                                                                  *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Nov 23, 2024>                                                                 *
 *  [Description] :      <Header file for GLCD12864 driver>                                             *
 ********************************************************************************************************/

#ifndef GLCD_H
#define GLCD_H

/*******************************************************************************
 *                                 Includes                                    *
 *******************************************************************************/

#include "../../MCAL/Inc/DIO.h"


/*******************************************************************************
 *                             Macro Declarations                              *
 *******************************************************************************/

// GLCD Dimensions
#define GLCD_WIDTH   128
#define GLCD_HEIGHT  64

#define PAGE_NUM	 8


/*******************************************************************************
 *                         Data Types Declaration                              *
 *******************************************************************************/

typedef enum
{
	GLCD_PIXEL_BLACK,
	GLCD_PIXEL_WHITE
	
}GLCD_PixelColor_t;

typedef struct
{
	DIO_PinConfig       GLCD_RS_PIN;
	DIO_PinConfig       GLCD_RW_PIN;
	DIO_PinConfig       GLCD_EN_PIN;
	DIO_PinConfig       GLCD_CS1_PIN;
	DIO_PinConfig       GLCD_CS2_PIN;
	DIO_PinConfig       GLCD_RST_PIN;
	DIO_PinConfig       GLCD_BACKLIGHT_PIN;
	DIO_PinLogic        GLCD_BACKLIGHT_STATE;
	
	DIO_PinConfig       GLCD_DATA_PINS[8];
	
}GLCD_Config_t;

/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/

// Function Prototypes
void GLCD_Init(GLCD_Config_t* GLCD);
void GLCD_Clear(GLCD_Config_t* GLCD);
void GLCD_WriteString(GLCD_Config_t* GLCD, uint8_t Page_No, const char* Str);
void GLCD_DrawBitmap(GLCD_Config_t* GLCD, const char *str);
void GLCD_SetCursor(GLCD_Config_t* GLCD, uint8 x, uint8 y);

#endif // GLCD_H