/*
* Mega16Test.c
*
* Created: 12/23/2016 2:20:38 PM
* Author : Mikheil Janiashvili
*/


#define F_CPU 7372800UL // Clock Speed 7372800UL 7.3
#define BAUD 57600
#define PRESCALLER F_CPU/16/BAUD-1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char value;

// Function Declarations
void uart_init(void);
void uart_send(unsigned char data);

int main(void)
{
	// Initialization UART Communication
	uart_init();

	// Setting Interrupts
	sei();

	// PORTS
	DDRC  = 0xFF;
	PORTC = 0x00;
	while(1)
	{
		_delay_ms(250);
	}
	return 0;
}

void uart_init ()
{
    // Set baud rate
    UBRR0H = (PRESCALLER >> 8);
    UBRR0L = PRESCALLER;
	// Turn on USART hardware (RX, TX)
	UCSR0B|= (1<<TXEN0) | (1<<RXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
    // Enable the USART Receive interrupt
    UCSR0B |= (1 << RXCIE0 );
}

void uart_send(unsigned char data){
	// Wait until last byte has been transmitted
	while((UCSR0A & (1<<UDRE0)) == 0);
	UDR0 = data;
}

void uart_send_buffer (const char *data)
{
	while (*data) {
		uart_send(*data++);
	}
}
 

ISR (USART_RX_vect)
{
	// Read byte in interrupt
	while(!(UCSR0A & (1<<RXC0)));
	value = UDR0;
	
	// Decide Port Pin Output
	switch (value)
	{
		case 'W':
			PORTC = 0b11111111;
		break;
		case 'A':
			PORTC = 0b00001000;
		break;
		case 'D':
			PORTC = 0b00100000;
		break;
		case 'S':
			PORTC = 0b00000000;
		break;
	}
}