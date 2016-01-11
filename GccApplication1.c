#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

// function to initialize UART
void uart_init (void)
{
	UBRRH = (BAUDRATE>>8);                      // shift the register right by 8 bits
	UBRRL = BAUDRATE;                           // set baud rate
	UCSRA|= (1<<U2X);
	UCSRB|= (1<<TXEN)|(1<<RXEN);                // enable receiver and transmitter
	UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);   // 8bit data format
}

void transmit(long i)
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
	unsigned char j;
	int i;
	UCSRA|=(1<<1);
	UCSRB|=(1<<2);
	UCSRB|=(1<<3);
	UCSRC=0b10000110;
	UBRRH=0b01001111;
	DDRB=0b11111111;
	
	while(1)
	{

		j=recieve();
		i=j;
		PORTB=i;

	}
}