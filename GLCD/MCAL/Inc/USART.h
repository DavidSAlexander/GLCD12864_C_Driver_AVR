
/********************************************************************************************************
*  [FILE NAME]   :      <USART.h>                                                                       *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <June 14, 2024>                                                                 *
*  [Description} :      <Header file for the AVR ATmega 2560 USART driver>                              *
*********************************************************************************************************/


#ifndef USART_H_
#define USART_H_

#include "../../Includes/STD_TYPES.h"
#include "../../Includes/DEVICE_CONFIG.h"
#include "../../Includes/BIT_MACROS.h"
#include "../../Includes/STD_LIBRARIES.h"
#include "Global_Interrupt.h"

#define USART_TX_BUFFER_SIZE 256
#define USART_RX_BUFFER_SIZE 256


#define UCSR0A_REG     SFR_MEM8(0xC0)
#define UCSR0B_REG     SFR_MEM8(0XC1)
#define UCSR0C_REG     SFR_MEM8(0xC2)

#define UCSR1A_REG     SFR_MEM8(0xC8)
#define UCSR1B_REG     SFR_MEM8(0XC9)
#define UCSR1C_REG     SFR_MEM8(0xCA)

#define UCSR2A_REG     SFR_MEM8(0xD0) 
#define UCSR2B_REG     SFR_MEM8(0XD1)
#define UCSR2C_REG     SFR_MEM8(0xD2)

#define UCSR3A_REG     SFR_MEM8(0x130)
#define UCSR3B_REG     SFR_MEM8(0X131)
#define UCSR3C_REG     SFR_MEM8(0x132)

#define UBRR0_REG      SFR_MEM16(0xC4)
#define UBRR0L_REG     SFR_MEM8(0xC4)
#define UBRR0H_REG     SFR_MEM8(0xC5)

#define UBRR1_REG      SFR_MEM16(0xCC)
#define UBRR1L_REG     SFR_MEM8(0xCC)
#define UBRR1H_REG     SFR_MEM8(0xCD)

#define UBRR2_REG      SFR_MEM16(0xD4)
#define UBRR2L_REG     SFR_MEM8(0xD4)
#define UBRR2H_REG     SFR_MEM8(0xD5)

#define UBRR3_REG      SFR_MEM16(0x134)
#define UBRR3L_REG     SFR_MEM8(0x134)
#define UBRR3H_REG     SFR_MEM8(0x135)

#define UDR0_REG       SFR_MEM8(0XC6)
#define UDR1_REG       SFR_MEM8(0XCE)
#define UDR2_REG       SFR_MEM8(0XD6)
#define UDR3_REG       SFR_MEM8(0X136)


/* UCSR0A */
#define RXC0_BIT       7
#define TXC0_BIT       6
#define UDRE0_BIT      5
#define FE0_BIT        4
#define DOR0_BIT       3
#define UPE0_BIT       2
#define U2X0_BIT       1
#define MPCM0_BIT      0

/* UCSR1A */
#define RXC1_BIT       7
#define TXC1_BIT       6
#define UDRE1_BIT      5
#define FE1_BIT        4
#define DOR1_BIT       3
#define UPE1_BIT       2
#define U2X1_BIT       1
#define MPCM1_BIT      0

/* UCSR2A */
#define RXC2_BIT       7
#define TXC2_BIT       6
#define UDRE2_BIT      5
#define FE2_BIT        4
#define DOR2_BIT       3
#define UPE2_BIT       2
#define U2X2_BIT       1
#define MPCM2_BIT      0

/* UCSR3A */
#define RXC3_BIT       7
#define TXC3_BIT       6
#define UDRE3_BIT      5
#define FE3_BIT        4
#define DOR3_BIT       3
#define UPE3_BIT       2
#define U2X3_BIT       1
#define MPCM3_BIT      0

/* UCSR0B */
#define RXCIE0_BIT     7
#define TXCIE0_BIT     6
#define UDRIE0_BIT     5
#define RXEN0_BIT      4
#define TXEN0_BIT      3
#define UCSZ02_BIT     2
#define RXB80_BIT      1
#define TXB80_BIT      0

/* UCSR1B */
#define RXCIE1_BIT     7
#define TXCIE1_BIT     6
#define UDRIE1_BIT     5
#define RXEN1_BIT      4
#define TXEN1_BIT      3
#define UCSZ12_BIT     2
#define RXB81_BIT      1
#define TXB81_BIT      0

/* UCSR2B */
#define RXCIE2_BIT     7
#define TXCIE2_BIT     6
#define UDRIE2_BIT     5
#define RXEN2_BIT      4
#define TXEN2_BIT      3
#define UCSZ22_BIT     2
#define RXB82_BIT      1
#define TXB82_BIT      0

/* UCSR3B */
#define RXCIE3_BIT     7
#define TXCIE3_BIT     6
#define UDRIE3_BIT     5
#define RXEN3_BIT      4
#define TXEN3_BIT      3
#define UCSZ32_BIT     2
#define RXB83_BIT      1
#define TXB83_BIT      0

/* UCSR0C */
#define UMSEL01_BIT    7
#define UMSEL00_BIT    6
#define UPM01_BIT      5
#define UPM00_BIT      4
#define USBS0_BIT      3
#define UCSZ01_BIT     2
#define UCSZ00_BIT     1
#define UCPOL0_BIT     0

/* UCSR1C */
#define UMSEL11_BIT    7
#define UMSEL10_BIT    6
#define UPM11_BIT      5
#define UPM10_BIT      4
#define USBS1_BIT      3
#define UCSZ11_BIT     2
#define UCSZ10_BIT     1
#define UCPOL1_BIT     0

/* UCSR2C */
#define UMSEL21_BIT    7
#define UMSEL20_BIT    6
#define UPM21_BIT      5
#define UPM20_BIT      4
#define USBS2_BIT      3
#define UCSZ21_BIT     2
#define UCSZ20_BIT     1
#define UCPOL2_BIT     0

/* UCSR3C */
#define UMSEL31_BIT    7
#define UMSEL30_BIT    6
#define UPM31_BIT      5
#define UPM30_BIT      4
#define USBS3_BIT      3
#define UCSZ31_BIT     2
#define UCSZ30_BIT     1
#define UCPOL3_BIT     0
 
#define USART_CHANNELS 4
 typedef enum
 {
	 USART_CHANNEL0,
	 USART_CHANNEL1,
	 USART_CHANNEL2,
	 USART_CHANNEL3
 }USART_Channel_t;
 
typedef enum
{
	USART_2400bps   = 2400,
	USART_4800bps   = 4800,
	USART_9600bps   = 9600,
	USART_14406bps  = 14406,
	USART_19200bps  = 19200,
	USART_38400bps  = 38400,
	USART_57600bps  = 57600,
	USART_115200bps = 115200,
	USART_128000bps = 128000,
	USART_256000bps = 256000
}USART_BuadRates_t;

typedef enum
{
	USART_ParityCheckDisabled,
	USART_ParityCheckEnabledEven = 2,
	USART_ParityCheckEnabledOdd
}USART_ParityCheck_t;

typedef enum
{
	USART_1BitStop,
	USART_2BitsStop
}USART_StopBits_t;

typedef enum
{
	USART_InterruptDisabled,
	USART_InterruptEnabled
}USART_Interrupt_t;

typedef enum
{
	USART_RisingTxFallingRxEdge,
	USART_RisingRxFallingTxEdge
}USART_ClockPolaritySelect_t;

typedef enum
{
	USART_AsynchronousMode,
	USART_SynchronousMode
}USART_OperationMode_t;

typedef enum
{
	USART_5BitsDataSize,
	USART_6BitsDataSize,
	USART_7BitsDataSize,
	USART_8BitsDataSize,
	USART_9BitsDataSize = 7
}USART_DataSize_t;

typedef enum
{
	USART_DoubleSpeedDisabled = 16,
	USART_DoubleSpeedEnabled  = 8
}USART_DoubleTransmissionSpeed_t;

typedef enum
{
	USART_StringUnavailable,
	USART_StringAvailable
}USART_StringStatus_t;

typedef struct
{
  USART_BuadRates_t				     USART_BaudRate;
  USART_DataSize_t					 USART_DataSize;
  USART_OperationMode_t				 USART_OperationMode;
  USART_ClockPolaritySelect_t		 USART_ClockPolarity;
  USART_Interrupt_t					 USART_InterruptStatus;
  USART_StopBits_t					 USART_StopBits;
  USART_ParityCheck_t				 USART_ParityCheck;
  USART_DoubleTransmissionSpeed_t    USART_DoubleSpeedStatus;
  USART_Channel_t                    USART_Channel;
  
}USART_Config_t;

Std_ReturnType USART_Init(const USART_Config_t* USARTcfg);
Std_ReturnType USART_Transmit_Byte(const USART_Config_t* USARTcfg, uint8 data);
Std_ReturnType USART_Receive_Byte(const USART_Config_t* USARTcfg, uint8* data);
Std_ReturnType USART_Transmit_String(const USART_Config_t* USARTcfg, const uint8* data);
Std_ReturnType USART_Receive_String(const USART_Config_t* USARTcfg, uint8* data);
Std_ReturnType USART_Flush(const USART_Config_t* USARTcfg);
Std_ReturnType USART_StringReady(const USART_Config_t* USARTcfg, USART_StringStatus_t* status);


#endif /* USART_H_ */