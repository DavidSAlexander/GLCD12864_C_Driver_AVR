
/********************************************************************************************************
*  [FILE NAME]   :      <USART.c>                                                                       *
*  [AUTHOR]      :      <David S. Alexander>                                                            *
*  [DATE CREATED]:      <Nov 17, 2024>                                                                  *
*  [Description] :      <Source file for the AVR ATmega 2560 USART driver>                              *
*********************************************************************************************************/

#include "../Inc/USART.h"
#include <string.h>
#include <math.h>

/* UCSRx Registers */
volatile uint8 *UCSRA_REG[]    = {&UCSR0A_REG, &UCSR1A_REG, &UCSR2A_REG, &UCSR3A_REG};
volatile uint8 *UCSRB_REG[]    = {&UCSR0B_REG, &UCSR1B_REG, &UCSR2B_REG, &UCSR3B_REG};	
volatile uint8 *UCSRC_REG[]    = {&UCSR0C_REG, &UCSR1C_REG, &UCSR2C_REG, &UCSR3C_REG};
	
/* UBRRx Registers */
volatile uint16 *UBRR_REG[]    = {&UBRR0_REG, &UBRR1_REG, &UBRR2_REG, &UBRR3_REG};
	
/* UDRx Registers */
volatile uint8 *UDR_REG[]      = {&UDR0_REG, &UDR1_REG, &UDR2_REG, &UDR3_REG};

static uint8 RX_Buffer[USART_CHANNELS][USART_RX_BUFFER_SIZE];
static volatile uint8 RX_BufferHead[USART_CHANNELS];
static volatile uint8 RX_BufferTail[USART_CHANNELS];
			
static uint8 TX_Buffer[USART_CHANNELS][USART_TX_BUFFER_SIZE];
static volatile uint8 TX_BufferHead[USART_CHANNELS];
static volatile uint8 TX_BufferTail[USART_CHANNELS];

static volatile uint8 TX_IndexBuffer[USART_CHANNELS];
static volatile uint8 RX_IndexBuffer[USART_CHANNELS];

static volatile boolean RX_DataIsReady[USART_CHANNELS];
static volatile boolean RX_DataIsAvailable[USART_CHANNELS];

static void USART_RX_InterruptHandler(USART_Channel_t Channel)
{
	if (((RX_BufferHead[Channel] + 1) % USART_RX_BUFFER_SIZE) != RX_BufferTail[Channel])
	{
		RX_Buffer[Channel][RX_BufferHead[Channel]] = *UDR_REG[Channel];
		RX_BufferHead[Channel] = (RX_BufferHead[Channel] + 1) % USART_RX_BUFFER_SIZE;
		if (BIT_IS_SET(*UCSRA_REG[Channel], FE0_BIT) || BIT_IS_SET(*UCSRA_REG[Channel], DOR0_BIT) || BIT_IS_SET(*UCSRA_REG[Channel], UPE0_BIT))
		{
			if (*UDR_REG[Channel]);      /* Clear error flags */
		}
	}
}

static void USART_UDRE_InterruptHandler(USART_Channel_t Channel)
{
	if (TX_BufferHead[Channel] != TX_BufferTail[Channel])
	{
		*UDR_REG[Channel] = (uint16)(TX_Buffer[TX_BufferTail[Channel]]);
		TX_BufferTail[Channel] = (TX_BufferTail[Channel] + 1) % USART_TX_BUFFER_SIZE;
	}
	CLEAR_BIT(*UCSRB_REG[Channel], UDRIE0_BIT);
}

Std_ReturnType USART_Init(const USART_Config_t* USARTcfg)
{
	Std_ReturnType ret = E_OK;
	
	uint8  UCSRA_Init = 0;
	uint8  UCSRB_Init = 0;
	uint8  UCSRC_Init = 0;
	uint16 UBRR_Init  = 0;
	
	uint8 Divisor = 16;
	if ( USARTcfg->USART_DoubleSpeedStatus == USART_DoubleSpeedEnabled && USARTcfg->USART_OperationMode == USART_AsynchronousMode ) Divisor = 8;
	
	/* Set the USART Data Size */
	UCSRC_Init |= (USARTcfg->USART_DataSize & 0x03) << UCSZ00_BIT;

	/* Set the USART Operation Mode */
	if (USARTcfg->USART_OperationMode == USART_SynchronousMode)
	{
		UCSRC_Init |= (1 << UMSEL00_BIT);
	}

	/* Set the USART Clock Polarity */
	if (USARTcfg->USART_ClockPolarity == USART_RisingRxFallingTxEdge)
	{
		UCSRC_Init |= (1 << UCPOL0_BIT);
	}

	/* Set the USART Stop Bits */
	if (USARTcfg->USART_StopBits == USART_2BitsStop)
	{
		UCSRC_Init |= (1 << USBS0_BIT);
	}

	/* Set the USART Parity Check */
	UCSRC_Init |= (USARTcfg->USART_ParityCheck << 4);

	/* Set the USART Double Transmission Speed */
	if (USARTcfg->USART_DoubleSpeedStatus == USART_DoubleSpeedEnabled)
	{
		UCSRA_Init |= (1 << U2X0_BIT);
	}

	/* Set the USART Interrupt Status */
	if (USARTcfg->USART_InterruptStatus == USART_InterruptEnabled)
	{
		ENABLE_GIE();
		UCSRB_Init |= (1 << RXCIE0_BIT) | (1 << TXCIE0_BIT) | (1 << UDRIE0_BIT);
	}

	UBRR_Init = round((float)F_CPU / Divisor / USARTcfg->USART_BaudRate - 1);
	*UBRR_REG[USARTcfg->USART_Channel] = UBRR_Init;

	*UCSRA_REG[USARTcfg->USART_Channel] = UCSRA_Init;
	*UCSRB_REG[USARTcfg->USART_Channel] = UCSRB_Init | (1 << RXEN0_BIT) | (1 << TXEN0_BIT);
	*UCSRC_REG[USARTcfg->USART_Channel] = UCSRC_Init;
	return ret;
}

Std_ReturnType USART_Transmit_Byte(const USART_Config_t* USARTcfg, uint8 data)
{
	Std_ReturnType ret = E_OK;
	if (USARTcfg->USART_InterruptStatus == USART_InterruptEnabled)
	{
		if ( (TX_BufferHead[USARTcfg->USART_Channel] + 1) % USART_TX_BUFFER_SIZE  != TX_BufferTail[USARTcfg->USART_Channel])
		{
			TX_Buffer[USARTcfg->USART_Channel][TX_BufferHead[USARTcfg->USART_Channel]] = data;
			TX_BufferHead[USARTcfg->USART_Channel] = (TX_BufferHead[USARTcfg->USART_Channel] + 1) % USART_TX_BUFFER_SIZE;
			SET_BIT(*UCSRB_REG[USARTcfg->USART_Channel], UDRIE0_BIT);
		}
		else return ret;
	}
	else if (USARTcfg->USART_InterruptStatus == USART_InterruptDisabled)
	{
		if ((TX_BufferHead[USARTcfg->USART_Channel] + 1) % USART_TX_BUFFER_SIZE  != TX_BufferTail[USARTcfg->USART_Channel])
		{
			TX_Buffer[USARTcfg->USART_Channel][TX_BufferHead[USARTcfg->USART_Channel]] = data;
			TX_BufferHead[USARTcfg->USART_Channel] = (TX_BufferHead[USARTcfg->USART_Channel] + 1) % USART_TX_BUFFER_SIZE;
		}
		
		while (TX_BufferHead[USARTcfg->USART_Channel] != TX_BufferTail[USARTcfg->USART_Channel])
		{
			*UDR_REG[USARTcfg->USART_Channel] = (uint16) (TX_Buffer[TX_BufferTail[USARTcfg->USART_Channel]]);
			TX_BufferTail[USARTcfg->USART_Channel] = (TX_BufferTail[USARTcfg->USART_Channel] + 1) % USART_TX_BUFFER_SIZE;
			while (BIT_IS_CLEAR(*UCSRA_REG[USARTcfg->USART_Channel],TXC0_BIT));
			SET_BIT(*UCSRA_REG[USARTcfg->USART_Channel],TXC0_BIT);
		}
	}
	return ret;
}

Std_ReturnType USART_Receive_Byte(const USART_Config_t* USARTcfg, uint8* data)
{
	Std_ReturnType ret = E_OK;
	uint8 BufferIndex = RX_BufferHead[USARTcfg->USART_Channel];
	if (USARTcfg->USART_InterruptStatus == USART_InterruptEnabled )
	{
		if (BufferIndex != RX_BufferTail[USARTcfg->USART_Channel])
		{
			*data = RX_Buffer[USARTcfg->USART_Channel][RX_BufferTail[USARTcfg->USART_Channel]];
			RX_BufferTail[USARTcfg->USART_Channel] = (RX_BufferTail[USARTcfg->USART_Channel] + 1) % USART_RX_BUFFER_SIZE;
			RX_DataIsReady[USARTcfg->USART_Channel] = TRUE;
		}
	}

	else if (USARTcfg->USART_InterruptStatus == USART_InterruptDisabled )
	{
		if (BufferIndex != RX_BufferTail[USARTcfg->USART_Channel])
		{
			*data = RX_Buffer[USARTcfg->USART_Channel][RX_BufferTail[USARTcfg->USART_Channel]];
			RX_BufferTail[USARTcfg->USART_Channel] = (RX_BufferTail[USARTcfg->USART_Channel] + 1) % USART_RX_BUFFER_SIZE;
			RX_DataIsReady[USARTcfg->USART_Channel] = TRUE;
		}
		else if ( BufferIndex == RX_BufferTail[USARTcfg->USART_Channel] )
		{
			// Wait for data to be received
			while(BIT_IS_CLEAR(*UCSRA_REG[USARTcfg->USART_Channel], RXC0_BIT));
			SET_BIT(*UCSRA_REG[USARTcfg->USART_Channel],TXC0_BIT);
			
			// Check for errors
			if (BIT_IS_SET(*UCSRA_REG[USARTcfg->USART_Channel], FE0_BIT) || BIT_IS_SET(*UCSRA_REG[USARTcfg->USART_Channel], DOR0_BIT) || BIT_IS_SET(*UCSRA_REG[USARTcfg->USART_Channel], UPE0_BIT))
			{
				if (*UDR_REG[USARTcfg->USART_Channel]);      // Clear error flags
				ret = E_NOT_OK;
			}
			else
			{
				if (((RX_BufferHead[USARTcfg->USART_Channel] + 1) % USART_RX_BUFFER_SIZE) != RX_BufferTail[USARTcfg->USART_Channel])
				{
					RX_Buffer[USARTcfg->USART_Channel][RX_BufferHead[USARTcfg->USART_Channel]] = *UDR_REG[USARTcfg->USART_Channel];
					RX_BufferHead[USARTcfg->USART_Channel] = (RX_BufferHead[USARTcfg->USART_Channel] + 1) % USART_RX_BUFFER_SIZE;
				}
			}
		}
	}
	return ret;
}

Std_ReturnType USART_Transmit_String(const USART_Config_t* USARTcfg, const uint8* data)
{
	Std_ReturnType ret = E_OK;
	uint8 Counter = ZERO_INIT;
	while (data[Counter] != '\0')
	{
		USART_Transmit_Byte(USARTcfg, data[Counter++]);
	}
	return ret;
}

Std_ReturnType USART_Receive_String(const USART_Config_t* USARTcfg, uint8* str)
{
	Std_ReturnType ret = E_OK;
	uint8 ReceivedCharacter = ZERO_INIT;
	uint8 strBuffer[USART_RX_BUFFER_SIZE];
	USART_Receive_Byte(USARTcfg, &ReceivedCharacter);
	if (  RX_DataIsReady[USARTcfg->USART_Channel] == TRUE )
	{
		strBuffer[RX_IndexBuffer[USARTcfg->USART_Channel]] = ReceivedCharacter;
		RX_DataIsReady[USARTcfg->USART_Channel] = FALSE;
		if ( strBuffer[RX_IndexBuffer[USARTcfg->USART_Channel]] == '\r' )
		{
			strBuffer[RX_IndexBuffer[USARTcfg->USART_Channel]] = '\0';
			memcpy(str, strBuffer, RX_IndexBuffer[USARTcfg->USART_Channel] + 1);
			RX_IndexBuffer[USARTcfg->USART_Channel] = ZERO_INIT;
			RX_DataIsAvailable[USARTcfg->USART_Channel] = TRUE;
		}
		
		else if ( strBuffer[RX_IndexBuffer[USARTcfg->USART_Channel]] != '\r' && strBuffer[RX_IndexBuffer[USARTcfg->USART_Channel]] != '\0' )
		{
			//str[RX_IndexBuffer] = ReceivedCharacter;
			RX_IndexBuffer[USARTcfg->USART_Channel] = ( RX_IndexBuffer[USARTcfg->USART_Channel] + 1 ) % USART_RX_BUFFER_SIZE;
		}
		
	}
	return ret;
}
Std_ReturnType USART_StringReady(const USART_Config_t* USARTcfg, USART_StringStatus_t* status)
{
	Std_ReturnType ret = E_OK;
	if (NULL == status || *status < USART_StringUnavailable || *status > USART_StringAvailable) 
	{
		ret = E_NOT_OK;
	}
	else
	{
		if (RX_DataIsAvailable[USARTcfg->USART_Channel] == TRUE)
		{
			*status = USART_StringAvailable;
			RX_DataIsAvailable[USARTcfg->USART_Channel] = FALSE;
		}
		else
		{
			*status = USART_StringUnavailable;
		}
	}
	return ret;
}
Std_ReturnType USART_Flush(const USART_Config_t* USARTcfg)
{
	while (*UCSRA_REG[USARTcfg->USART_Channel] & (1<<RXC0) ) if (*UDR_REG[USARTcfg->USART_Channel]);
	return E_OK;
}

ISR(USART0_RX_vect)
{
	USART_RX_InterruptHandler(USART_CHANNEL0);
}

ISR(USART1_RX_vect)
{
	USART_RX_InterruptHandler(USART_CHANNEL1);
}

ISR(USART2_RX_vect)
{
	USART_RX_InterruptHandler(USART_CHANNEL2);
}

ISR(USART3_RX_vect)
{
	USART_RX_InterruptHandler(USART_CHANNEL3);
}


ISR(USART0_TX_vect)
{
	//TX_BufferHead = (TX_BufferHead + 1) % RX_BUFFER_SIZE;
}
ISR(USART0_UDRE_vect)
{
	USART_UDRE_InterruptHandler(USART_CHANNEL0);
}

ISR(USART1_UDRE_vect)
{
	USART_UDRE_InterruptHandler(USART_CHANNEL1);
}

ISR(USART2_UDRE_vect)
{
	USART_UDRE_InterruptHandler(USART_CHANNEL2);
}

ISR(USART3_UDRE_vect)
{
	USART_UDRE_InterruptHandler(USART_CHANNEL3);
} 


