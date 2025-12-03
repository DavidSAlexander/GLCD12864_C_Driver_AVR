
/********************************************************************************************************
 *  [FILE NAME]   :      <DIO.h>                                                                        *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 14, 2024>                                                                *
 *  [Description} :      <Header file for the AVR ATmega 2560 DIO driver>                               *
 ********************************************************************************************************/
 
#ifndef DIO_H
#define	DIO_H

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "../../Includes/STD_TYPES.h"
#include "../../Includes/DEVICE_CONFIG.h"
#include "../../Includes/BIT_MACROS.h"
#include "../../Includes/STD_LIBRARIES.h"

/*******************************************************************************
 *                             Macro Declarations                              *
 *******************************************************************************/


/* Port L */
#define DIO_PINL    SFR_MEM8(0x109)
#define DIO_DDRL    SFR_MEM8(0x10A)
#define DIO_PORTL   SFR_MEM8(0x10B)

/* Port K */
#define DIO_PINK    SFR_MEM8(0x106)
#define DIO_DDRK    SFR_MEM8(0x107)
#define DIO_PORTK   SFR_MEM8(0x108)

/* Port J */
#define DIO_PINJ    SFR_MEM8(0x103)
#define DIO_DDRJ    SFR_MEM8(0x104)
#define DIO_PORTJ   SFR_MEM8(0x105)

/* Port H */
#define DIO_PINH    _SFR_MEM8(0x100)
#define DIO_DDRH    _SFR_MEM8(0x101)
#define DIO_PORTH   _SFR_MEM8(0x102)

/* Port G */
#define DIO_PING    SFR_IO8(0x12)
#define DIO_DDRG    SFR_IO8(0x13)
#define DIO_PORTG   SFR_IO8(0x14)

/* Port F */
#define DIO_PINF    SFR_IO8(0x0F)
#define DIO_DDRF    SFR_IO8(0x10)
#define DIO_PORTF   SFR_IO8(0x11)

/* Port E */
#define DIO_PINE    SFR_IO8(0x0C)
#define DIO_DDRE    SFR_IO8(0x0D)
#define DIO_PORTE   SFR_IO8(0x0E)

/* Port D */
#define DIO_PIND    SFR_IO8(0x09)  
#define DIO_DDRD    SFR_IO8(0x0A)
#define DIO_PORTD   SFR_IO8(0x0B)

/* Port C */
#define DIO_PINC    SFR_IO8(0x06)
#define DIO_DDRC    SFR_IO8(0x07)
#define DIO_PORTC   SFR_IO8(0x08)

/* Port B */
#define DIO_PINB    SFR_IO8(0X03)
#define DIO_DDRB    SFR_IO8(0x04)
#define DIO_PORTB   SFR_IO8(0x05)

/* Port A */
#define DIO_PINA    SFR_IO8(0X00)
#define DIO_DDRA    SFR_IO8(0X01)
#define DIO_PORTA   SFR_IO8(0X02)

/*******************************************************************************
 *                         Data Types Declaration                              *
 *******************************************************************************/

typedef enum
{
	LOW,
	HIGH
} DIO_PinLogic;

typedef enum
{
	DIO_Pulldown,
	DIO_InternalPullup
} DIO_PinInputRes;

typedef enum
{
    PIN_INPUT,
    PIN_OUTPUT
} DIO_PinDirection;

typedef enum
{
    PORT_INPUT  = 0,
    PORT_OUTPUT = 0xFF
} DIO_PortDirection;

typedef enum
{
    PIN0_INDEX,
    PIN1_INDEX,
    PIN2_INDEX,
    PIN3_INDEX,
    PIN4_INDEX,
    PIN5_INDEX,
    PIN6_INDEX,
    PIN7_INDEX
} DIO_PinIndex;

typedef enum
{
    PORTA_INDEX,
    PORTB_INDEX,
    PORTC_INDEX,
    PORTD_INDEX,
	PORTE_INDEX,
	PORTF_INDEX,
	PORTG_INDEX,
	PORTH_INDEX,
	PORTJ_INDEX,
	PORTK_INDEX,
	PORTL_INDEX
} DIO_PortIndex;

typedef struct
{
    uint8 PORTx  : 4;
    uint8 PINx   : 3;
    uint8 DIRx   : 1;
    uint8 STATEx : 1;

} DIO_PinConfig;

		
/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/

Std_ReturnType DIO_PinDirSetup(const DIO_PinConfig *Pin_Config);
Std_ReturnType DIO_PinDirStatus(const DIO_PinConfig *Pin_Config, DIO_PinDirection *Dir_Status);
Std_ReturnType DIO_PinWrite(const DIO_PinConfig *Pin_Config, DIO_PinLogic Logic);
Std_ReturnType DIO_PinRead(const DIO_PinConfig *Pin_Config, DIO_PinLogic *Logic);
Std_ReturnType DIO_PinToggle(const DIO_PinConfig *Pin_Config);

Std_ReturnType DIO_PortDirSetup(DIO_PortIndex Port_Config, uint8 direction);
Std_ReturnType DIO_PortDirStatus(DIO_PortIndex Port_Status, uint8 *Status);
Std_ReturnType DIO_PortWrite(const DIO_PortIndex Port_Index, uint8 Logic);
Std_ReturnType DIO_PortRead(const DIO_PortIndex Port_Index, uint8 *Logic);
Std_ReturnType DIO_PortToggle(const DIO_PortIndex Port_Index);
Std_ReturnType DIO_PortROL(DIO_PortIndex Port, uint8 NUM);
Std_ReturnType DIO_PortROR(DIO_PortIndex Port, uint8 NUM);

#endif	/* DIO_H */
