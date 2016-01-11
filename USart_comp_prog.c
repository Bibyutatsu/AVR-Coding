#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

volatile unsigned char reievebyte;
// function to initialize UART
void uart_init (void)
{
	UBRRH = (unsigned char)(BAUDRATE>>8);                      // shift the register right by 8 bits
	UBRRL = (unsigned char)BAUDRATE;                           // set baud rate
	//UCSRA|= (1<<U2X);
	UCSRB|= (1<<TXEN)|(1<<RXEN)|(1<<RXCIE);                // enable receiver and transmitter
	UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);   // 8bit data format
}

void transmit(unsigned char i)
{
	while(!(UCSRA&(1<<5)))
	{}
	UDR=i;
}

int recieve(void)
{
	while(!(UCSRA&(1<<7)))
	{}
	return UDR;
}

int main()
{
	uart_init();
	
	sei();
	DDRB=0b11111111;
	
	while(1)
	{
		//transmit('A');
		//_delay_ms(2000);

	}
}

ISR(USART_RXC_vect)
{
	reievebyte=UDR;
	PORTB^=1<<PINB0;
	transmit(reievebyte);
}