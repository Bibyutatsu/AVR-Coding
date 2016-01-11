#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

volatile unsigned char reievebyte;

int i=0;										//global variable
int a[5];
void uart_init (void)						// function to initialize UART
{
	UBRRH = (unsigned char)(BAUDRATE>>8);                      // shift the register right by 8 bits
	UBRRL = (unsigned char)BAUDRATE;                           // set baud rate
	//UCSRA|= (1<<U2X);
	UCSRB|= (1<<TXEN)|(1<<RXEN)|(1<<RXCIE);                // enable receiver and transmitter
	UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);   // 8bit data format
}

void transmit(unsigned char j)
{
	while(!(UCSRA&(1<<5)))
	{}
	UDR=j;
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
	
	//int i,j;
	sei();
	
	DDRB=0b11111111;
	DDRD=0xff;
	
	TCCR1A|=1<<WGM11 |1<<COM1A1 | 1<<COM1A0;
	TCCR1B|=1<<WGM12 | 1<<WGM13 | 1<< CS11 | 1<<CS10;
	
	ICR1=4999;
	
	while(1)
	{
		
	}
}

ISR(USART_RXC_vect)
{
	int j,k,angle=0,timewidth;
	reievebyte=UDR;
	transmit(reievebyte);
	if(reievebyte!=32)
	{
		a[i]=reievebyte-48;
		++i;
	}
	
	if(reievebyte==32)
	{
		transmit('c');
		j=i;
		i=0;
		for(k=0;k<j;k++)
		{
			angle=(angle*10)+a[k];
			a[k]=0;
		}
		
		timewidth=0.6+(1.8*angle/180);
		OCR1A=(int)(ICR1-((ICR1+1)*timewidth/20)-1);
		
	}
	
}