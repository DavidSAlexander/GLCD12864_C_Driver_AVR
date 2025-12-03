/********************************************************************************************************
 *  [FILE NAME]   :      <GLCD12864.c>                                                                  *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Nov 23, 2024>                                                                 *
 *  [Description] :      <Source file for GLCD12864 driver>                                             *
 ********************************************************************************************************/

#include <avr/pgmspace.h>
#include "../Inc/GLCD12864.h"
#include "../Inc/Font_Header.h"


static void GLCD_WriteCommand(GLCD_Config_t* GLCD, char cmd);
static void GLCD_Toggle_CS(GLCD_Config_t* GLCD);
static void GLCD_WriteData(GLCD_Config_t* GLCD, uint8 Data);

void GLCD_Init(GLCD_Config_t* GLCD) 
{
	/* Configure control and data ports */
	GLCD->GLCD_RS_PIN.DIRx   = PIN_OUTPUT;
	GLCD->GLCD_RS_PIN.STATEx = LOW;
	DIO_PinDirSetup(&(GLCD->GLCD_RS_PIN));
	
	GLCD->GLCD_EN_PIN.DIRx   = PIN_OUTPUT;
	GLCD->GLCD_EN_PIN.STATEx = LOW;
	DIO_PinDirSetup(&(GLCD->GLCD_EN_PIN));
	
	GLCD->GLCD_RW_PIN.DIRx   = PIN_OUTPUT;
	GLCD->GLCD_RW_PIN.STATEx = LOW;
	DIO_PinDirSetup(&(GLCD->GLCD_RW_PIN));
	
	GLCD->GLCD_RST_PIN.DIRx   = PIN_OUTPUT;
	GLCD->GLCD_RST_PIN.STATEx = LOW;
	DIO_PinDirSetup(&(GLCD->GLCD_RST_PIN));
	
	GLCD->GLCD_CS1_PIN.DIRx   = PIN_OUTPUT;
	GLCD->GLCD_CS1_PIN.STATEx = LOW;
	DIO_PinDirSetup(&(GLCD->GLCD_CS1_PIN));
	
	GLCD->GLCD_CS2_PIN.DIRx   = PIN_OUTPUT;
	GLCD->GLCD_CS2_PIN.STATEx = LOW;
	DIO_PinDirSetup(&(GLCD->GLCD_CS2_PIN));
	
	GLCD->GLCD_BACKLIGHT_PIN.DIRx   = PIN_OUTPUT;
	GLCD->GLCD_BACKLIGHT_PIN.STATEx = GLCD->GLCD_BACKLIGHT_STATE;
	DIO_PinDirSetup(&(GLCD->GLCD_BACKLIGHT_PIN));
	
	uint8 Counter = 0;
	for (;Counter < 8; Counter++)
	{
		GLCD->GLCD_DATA_PINS[Counter].DIRx   = PIN_OUTPUT;
		GLCD->GLCD_DATA_PINS[Counter].STATEx = LOW;
		DIO_PinDirSetup(&(GLCD->GLCD_DATA_PINS[Counter]));
	}
	
	/* Select both left & right half of display */
	DIO_PinWrite(&(GLCD->GLCD_CS1_PIN), HIGH);
	DIO_PinWrite(&(GLCD->GLCD_CS2_PIN), HIGH);
	DIO_PinWrite(&(GLCD->GLCD_RST_PIN), HIGH);
	DIO_PinWrite(&(GLCD->GLCD_BACKLIGHT_PIN), HIGH);
	
	_delay_ms(20);
	
	/* Initialize display commands (depends on your specific GLCD controller) */
	GLCD_WriteCommand(GLCD, 0x3E);                      /* Display ON */
	GLCD_WriteCommand(GLCD, 0x40);                      /* Set Y address (column) */
	GLCD_WriteCommand(GLCD, 0xB8);                      /* Set X address (page) */
	GLCD_WriteCommand(GLCD, 0xC0);					    /* Set z address (start line = 0) */
	GLCD_WriteCommand(GLCD, 0x3F);						/* Display ON */
}

void GLCD_Clear(GLCD_Config_t* GLCD) 
{	
	/* Select both left & right half of display */
	DIO_PinWrite(&(GLCD->GLCD_CS1_PIN), HIGH);
	DIO_PinWrite(&(GLCD->GLCD_CS2_PIN), HIGH);
	
	/* Send clear command to GLCD */
	for (uint8_t Page = 0; Page < PAGE_NUM; Page++) 
	{
		GLCD_WriteCommand(GLCD, (0xB8) + Page);           /* Set page address */
		
		for (uint8_t Col = 0; Col < 64; Col++) 
		{
			GLCD_WriteData(GLCD, 0x00);
		}
	}
	GLCD_WriteCommand(GLCD, 0x40);                      /* Set column to 0  */
	GLCD_WriteCommand(GLCD, 0xB8);					    /* Set x address (page = 0) */
}

/* GLCD string write function */
void GLCD_WriteString(GLCD_Config_t* GLCD, uint8_t Page_No, const char* Str)
{
	unsigned int Counter, Col, Page = ((0xB8) + Page_No);
	unsigned int Y_Address = 0;
	float Page_inc = 0.5;

    DIO_PinWrite(&(GLCD->GLCD_CS1_PIN), HIGH);
    DIO_PinWrite(&(GLCD->GLCD_CS2_PIN), LOW);
	
	GLCD_WriteCommand(GLCD, Page);          // Set the page address
	for (Counter = 0; Str[Counter] != 0; Counter++)
	{
		if (Y_Address > (1024 - (((Page_No) * 128) + FontWidth))) break;    /* Check Whether Total Display get overflowed */
		
		if (Str[Counter] != 32)								                /* Check whether character is not a SPACE */
		{
			for (Col = 1; Col <= FontWidth; Col++)
			{
				if ((Y_Address + Col) == (128 * ((int)(Page_inc + 0.5))))	/* If yes then check whether it overflow from right side of display */
				{
					if (Col == FontWidth)	                              	/* Also check and break if it overflow after 5th column */
					break;
					GLCD_WriteCommand(GLCD, 0x40);				            /* If not 5th and get overflowed then change Y address to START column */
					Y_Address = Y_Address + Col;	                        /* Increment Y address count by column no. */
					GLCD_Toggle_CS(GLCD);
					GLCD_WriteCommand(GLCD, Page + Page_inc);	            /* Execute command for page change */
					Page_inc = Page_inc + 0.5;		                        /* Increment Page No. by half */
				}
			}
		}
		if (Y_Address > (1024 - (((Page_No) * 128) + FontWidth))) break;    /* Check Whether Total Display get overflowed */
		
		if((font[((Str[Counter] - 32) * FontWidth) + 4]) == 0 || Str[Counter] == 32) /* Check whether character is SPACE or character last column is zero */
		{
			for(Col = 0; Col < FontWidth; Col++)
			{
				GLCD_WriteData(GLCD, font[Str[Counter] - 32][Col]);	/* If yes then then print character */
				if((Y_Address + 1) % 64 == 0)			           	/* check whether it gets overflowed  from either half of side */
				{
					GLCD_Toggle_CS(GLCD);
					GLCD_WriteCommand(GLCD, (Page + Page_inc));	/* Execute command for page change */
					Page_inc = Page_inc + 0.5;		/* Increment Page No. by half */
				}
				Y_Address++;						/* Increment Y_address count per column */
			}
		}
		else										/* If character is not SPACE or character last column is not zero */
		{
			for(Col = 0; Col < FontWidth; Col++)
			{
				GLCD_WriteData(GLCD, font[Str[Counter] - 32][Col]); /* Then continue to print hat char */
				if((Y_Address + 1) % 64 == 0)				/* check whether it gets overflowed  from either half of side */
				{
					GLCD_Toggle_CS(GLCD);
					GLCD_WriteCommand(GLCD, (Page+Page_inc));	/* Execute command for page change */
					Page_inc = Page_inc + 0.5;		/* Increment Page No. by half */
				}
				Y_Address++;						/* Increment Y_address count per column */
			}
			GLCD_WriteData(GLCD, 0);							/* Add one column of zero to print next character next of zero */
			Y_Address++;							/* Increment Y_address count for last added zero */
			if((Y_Address) % 64 == 0)					/* check whether it gets overflowed  from either half of side */
			{
				GLCD_Toggle_CS(GLCD);
				GLCD_WriteCommand(GLCD, (Page+Page_inc));		/* Execute command for page change */
				Page_inc = Page_inc + 0.5;			/* Increment Page No. by half */
			}
		}
	}
	GLCD_WriteCommand(GLCD, 0x40);								/* Set Y address (column=0) */	
}

void GLCD_DrawBitmap(GLCD_Config_t* GLCD, const char *Bitmap)					/* GLCD string write function */
{
	int column, page, page_add = 0xB8, k = 0;
	float page_inc = 0.5;
	char byte;
	/* Select first left half of display to start print */
	DIO_PinWrite(&(GLCD->GLCD_CS1_PIN), HIGH);
	DIO_PinWrite(&(GLCD->GLCD_CS2_PIN), LOW);	
	
	for(page = 0; page < 16; page++)										    /* Print 16 pages i.e. 8 page of each half of display */
	{
		for(column = 0; column < 64; column++)
		{
			byte = pgm_read_byte(&Bitmap[k + column]);
			GLCD_WriteData(GLCD, byte); 		          		     			/* Print 64 column of each page */
		}
		GLCD_Toggle_CS(GLCD);
		
		GLCD_WriteCommand(GLCD, (page_add + page_inc));		                	/* Increment page address by 1 each after two page successful write */
		page_inc = page_inc + 0.5;
		k = k + 64;																/* Increment pointer by 64 after each page print */
	}
	GLCD_WriteCommand(GLCD, 0x40);												/* Set Y address (column=0) */
	GLCD_WriteCommand(GLCD, 0xB8);												/* Set x address (page=0) */
}

void GLCD_SetCursor(GLCD_Config_t* GLCD, uint8_t x, uint8_t y)
{
	GLCD_WriteCommand(GLCD, 0xB8 | y);  // Set the Y (page address)
	GLCD_WriteCommand(GLCD, 0x40 | x);  // Set the X (column address)
}

void GLCD_WriteCommand(GLCD_Config_t* GLCD, char cmd)
{	
	/* Write command data */
	uint8 Counter = 0;
	for (;Counter < 8; Counter++)
	{
		DIO_PinWrite(&(GLCD->GLCD_DATA_PINS[Counter]), READ_BIT(cmd, Counter));
	}
	
	/* Set RS low for command */
	DIO_PinWrite(&(GLCD->GLCD_RS_PIN), LOW);
	
	/* Set RW low for command */
	DIO_PinWrite(&(GLCD->GLCD_RW_PIN), LOW);
	
	/* Pulse enable */
	DIO_PinWrite(&(GLCD->GLCD_EN_PIN), HIGH);
	_delay_us(1);
	DIO_PinWrite(&(GLCD->GLCD_EN_PIN), LOW);
	_delay_us(1);
}

void GLCD_WriteData(GLCD_Config_t* GLCD, uint8 Data)
{
	/* Write data */
	uint8 Counter = 0;
	for (;Counter < 8; Counter++)
	{
		DIO_PinWrite(&(GLCD->GLCD_DATA_PINS[Counter]), READ_BIT(Data, Counter));
	}
	
	/* Set RS high for data */
	DIO_PinWrite(&(GLCD->GLCD_RS_PIN), HIGH);
	
	DIO_PinWrite(&(GLCD->GLCD_RW_PIN), LOW);
	
	/* Pulse enable */
	DIO_PinWrite(&(GLCD->GLCD_EN_PIN), HIGH);
	_delay_us(1);
	DIO_PinWrite(&(GLCD->GLCD_EN_PIN), LOW);
	_delay_us(1);
}

void GLCD_Toggle_CS(GLCD_Config_t* GLCD)
{
	DIO_PinToggle(&(GLCD->GLCD_CS1_PIN));
	DIO_PinToggle(&(GLCD->GLCD_CS2_PIN));
}