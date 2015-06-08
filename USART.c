/*
* USART.c
*
*  Created on: 8 06 2015
*      Author: paku
*
* This is a free software, under MIT License.
*
* You may use simple printf() family function with this USART interface.
*
* example:
* FILE stream = FDEV_SETUP_STREAM(USART_vSendByte,USART_vReceiveByte, _FDEV_SETUP_RW);
* stdin=&stream;
* stdout=&stream;
* print("Test USART\n");
*/

#include "USART.h"


void USART_vInit(void)

{

// Set baud rate.
// Consult your ATmega documentation
// Samples below given for 328p at 16MHz.
//#define BAUDRATE 103 // 9,6k
#define BAUDRATE 16  //57.6k
//#define BAUDRATE 8	 //115.2k

// Add your ATmega definition below
#if defined (__AVR_ATmega328P__)
#define UBRRH UBRR0H
#define UBRRL UBRR0L
#define UCSRB UCSR0B
#define RXEN RXEN0
#define TXEN TXEN0
#define UCSRC UCSR0C
#define URSEL USBS0
#define UCSZ0 UCSZ00
#define STOPBITMASK 0
#else //default is ATmega32
#define STOPBITMASK 1
#endif

//set baudrate
UBRRH = (uint8_t)(BAUDRATE>>8);
UBRRL = (uint8_t)BAUDRATE;

// Enable receiver and transmitter
UCSRB = (1<<RXEN)|(1<<TXEN);

// Set frame format to 8 data bits, no parity, 1 stop bit
UCSRC = (STOPBITMASK<<URSEL)|(3<<UCSZ0);

//UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);

}


void USART_vSendByte(uint8_t u8Data)
{
//define your AVR here
#if defined (__AVR_ATmega328P__)
#define	UCSRA UCSR0A
#define UDRE UDRE0
#define UDR UDR0
#endif

// Wait if a byte is being transmitted
while((UCSRA&(1<<UDRE)) == 0);

// Transmit data
UDR = u8Data;

}


uint8_t USART_vReceiveByte()
{
//define your AVR here
#if defined (__AVR_ATmega328P__)
#define RXC RXC0
#endif

// Wait until a byte has been received
while((UCSRA&(1<<RXC)) == 0);

// Return received data
return UDR;

}


