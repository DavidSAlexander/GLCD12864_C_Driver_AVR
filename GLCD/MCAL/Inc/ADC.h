/********************************************************************************************************
 *  [FILE NAME]   :      <ADC.h>                                                                        *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <June 12, 2023>                                                                *
 *  [Description} :      <Header file for the AVR ADC driver>                                           *
 ********************************************************************************************************/

#ifndef ADC_H_
#define ADC_H_

/*******************************************************************************
 *                                Includes                                     *
 *******************************************************************************/

#include "../../Includes/STD_TYPES.h"
#include "../../Includes/DEVICE_CONFIG.h"
#include "../../Includes/BIT_MACROS.h"
#include "../../Includes/STD_LIBRARIES.h"
#include "Global_Interrupt.h"

/*******************************************************************************
 *                             Macro Declarations                              *
 *******************************************************************************/

/* ADC Registers */
#define ADC_REG      SFR_MEM16(0x78)  /* ADC Data Register */   
#define ADCL_REG     SFR_MEM8(0x78)   /* ADC Data Register Low */
#define ADCH_REG     SFR_MEM8(0x79)   /* ADC Data Register High */
#define ADCSRA_REG   SFR_MEM8(0x7A)   /* ADC Control and Status Register A */
#define ADCSRB_REG   SFR_MEM8(0x7B)   /* ADC Control and Status Register B */
#define ADMUX_REG    SFR_MEM8(0x7C)   /* ADC Multiplexer Selection Register */
#define DIDR0_REG    SFR_MEM8(0x7E)   /* ADC Digital Input Disable Register 0 */
#define DIDR1_REG    SFR_MEM8(0x7F)   /* ADC Digital Input Disable Register 1 */
#define DIDR2_REG    SFR_MEM8(0x7D)   /* ADC Digital Input Disable Register 2 */

/* ADCSRA */
#define ADPS0_Bit   0
#define ADPS1_Bit   1
#define ADPS2_Bit   2
#define ADIE_Bit    3
#define ADIF_Bit    4
#define ADATE_Bit   5
#define ADSC_Bit    6
#define ADEN_Bit    7

/* ADCSRB */
#define ADTS0_Bit   0
#define ADTS1_Bit   1
#define ADTS2_Bit   2
#define MUX5_Bit    3
#define ACME_Bit    6

/* ADMUX */
#define MUX0_Bit    0
#define MUX1_Bit    1
#define MUX2_Bit    2
#define MUX3_Bit    3
#define MUX4_Bit    4
#define ADLAR_Bit   5
#define REFS0_Bit   6
#define REFS1_Bit   7

/* DIDR0 */
#define ADC0D_Bit   0
#define ADC1D_Bit   1
#define ADC2D_Bit   2
#define ADC3D_Bit   3
#define ADC4D_Bit   4
#define ADC5D_Bit   5
#define ADC6D_Bit   6
#define ADC7D_Bit   7

/* DIDR1 */
#define AIN0D_Bit   0
#define AIN1D_Bit   1

/* DIDR2 */
#define ADC8D_Bit   0
#define ADC9D_Bit   1
#define ADC10D_Bit  2
#define ADC11D_Bit  3
#define ADC12D_Bit  4
#define ADC13D_Bit  5
#define ADC14D_Bit  6
#define ADC15D_Bit  7


#define MIN_ADC_FREQ 200000UL
#define MAX_ADC_FREQ 1000000UL


/*******************************************************************************
 *                         Data Types Declaration                              *
 *******************************************************************************/

/* ADC channel index */
typedef enum
{
	ADC_CHANNEL0,        /* ADC channel 0  */
	ADC_CHANNEL1,        /* ADC channel 1  */
	ADC_CHANNEL2,        /* ADC channel 2  */
	ADC_CHANNEL3,        /* ADC channel 3  */
	ADC_CHANNEL4,        /* ADC channel 4  */
	ADC_CHANNEL5,        /* ADC channel 5  */
	ADC_CHANNEL6,        /* ADC channel 6  */
	ADC_CHANNEL7,        /* ADC channel 7  */
	ADC_CHANNEL8 = 32,   /* ADC channel 8  */
	ADC_CHANNEL9,        /* ADC channel 9  */
	ADC_CHANNEL10,       /* ADC channel 10 */
	ADC_CHANNEL11,       /* ADC channel 11 */
	ADC_CHANNEL12,       /* ADC channel 12 */
	ADC_CHANNEL13,       /* ADC channel 13 */
	ADC_CHANNEL14,       /* ADC channel 14 */
	ADC_CHANNEL15,       /* ADC channel 15 */
}ADC_Index;

/* ADC Division Factor Select */
typedef enum
{
	CLK_2_,         /* Clock / 2 */
	CLK_2,          /* Clock / 2 */
	CLK_4,          /* Clock / 4 */
	CLK_8,          /* Clock / 8 */
	CLK_16,         /* Clock / 16 */
	CLK_32,         /* Clock / 32 */
	CLK_64,         /* Clock / 64 */
	CLK_128         /* Clock / 128 */
}ADC_Prescaller;

/* ADC Auto Trigger Source Select */
typedef enum
{
	Free_Runnung,          /* Free Running mode */
	Analog_Comparator,     /* Analog Comparator */
	External_Interrupt,    /* External Interrupt Request 0 */
	TMR0_Compare_Match_A,  /* Timer/Counter0 Compare Match A */
	TMR0_OverFlow,         /* Timer/Counter0 Overflow */
	TMR1_Compare_Match_B,  /* Timer/Counter1 Compare Match B */
	TMR1_OverFlow,         /* Timer/Counter1 Overflow */
	TMR1_Capture           /* Timer/Counter1 Capture Event */
}ADC_Trigger_Source;

/* ADC result adjustment select */
typedef enum
{
	Right_Adjusted, /* Right-adjusted result */
	Left_Adjusted   /* Left-adjusted result */
}ADC_ResultAdjust;

/* ADC interrupt select */
typedef enum
{
	ADCInterruptDisabled,  /* Interrupt disabled */
	ADCInterruptEnabled    /* Interrupt enabled */
}ADC_Interrupt;

/* ADC voltage reference options */
typedef enum
{
	External_AREF,      /* External AREF voltage reference */
	External_AVCC,      /* External AVCC voltage reference */
	Internal_VREF = 3   /* Internal VREF voltage reference */
}ADC_VoltageReference;

/* ADC Multiplixer options */
typedef struct
{
	uint8 MUX_bits  :5;
	uint8 ADLAR_bit :1;
	uint8 REFS_bits :2;
}ADMUX_t;

/* ADC ADCSRA options */
typedef struct
{
	uint8 ADPS_bits :3;
	uint8 ADIE_bit  :1;
	uint8 ADIF_bit  :1;
	uint8 ADATE_bit :1;
	uint8 ADSC_bit  :1;
	uint8 ADEN_bit  :1;
}ADCSRA_t;

/* ADC ADCSRB options */
typedef struct
{
	uint8 ADTS_bits :3;
	uint8 MUX5_bit  :1;
	uint8 ACME_bit  :1;
}ADCSRB_t;

/* ADC channel configuration */
typedef struct
{
	ADC_ResultAdjust      ResultAdjust;        /* ADC result adjustment */
	ADC_Prescaller        Prescaller;          /* ADC Prescaller */
	ADC_VoltageReference  VoltageReference;    /* ADC voltage reference */
	ADC_Interrupt         InturruptMode;       /* ADC interrupt mode */
	ADCSRA_t              ADCSRA_Bits;         /* ADC ADCSRA Bits */
	ADMUX_t               ADMUX_Bits;          /* ADC Multiplixer Bits */
}ADC_Channel;

/*******************************************************************************
 *                            Functions Declaration                            *
 *******************************************************************************/

/* Initialize ADC module */
Std_ReturnType ADC_Init(ADC_Channel *ADC_Num);

/* Read from ADC module */
Std_ReturnType ADC_Read(ADC_Index ADC_Ch, uint16 *ADC_Result);

#endif /* ADC_H_ */