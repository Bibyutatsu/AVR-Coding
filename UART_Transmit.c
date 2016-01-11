#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

// function to initialize UART
void uart_init (void)
{
	UBRRH = (unsigned char)(BAUDRATE>>8);                      // shift the register right by 8 bits
	UBRRL = BAUDRATE;                           // set baud rate
	UCSRA|= (1<<U2X);
	UCSRB|= (1<<TXEN)|(1<<RXEN);                // enable receiver and transmitter
	UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1)|(1<<USBS);   // 8bit data format
}

void transmit(unsigned char i)
{
	while(!(UCSRA&(1<<UDRE)))
	{}
	UDR=i;
}

unsigned char recieve(void)
{
	while(!(UCSRA&(1<<7)))
	{}
	return UDR;
}

int main()
{
	unsigned char i;
	uart_init();
	DDRA=0x00;
	DDRB=0xff;
	ADCSRA=0b11000111;
	ADMUX=0b00100000;
	while(1)
	{
		//ADCSRA=0b11000111;
		//while(ADCSRA&(1<<6))
		//{}
			for(i=0;i<255;i++)
			{i=(unsigned char)i;
			transmit(i);
			_delay_ms(100);}
			//PORTB=ADCH;
			i=0;
			
	}
}