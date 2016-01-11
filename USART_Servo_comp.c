#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR

#define SERVO_0 0.6
#define SERVO_90 1.5
#define SERVO_180 2.4


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
	
	//int i,j;
	
	sei();
	
	DDRB=0b11111111;
	DDRD=0xff;
	
	TCCR1A|=1<<WGM11 |1<<COM1A1 | 1<<COM1A0;
	TCCR1B|=1<<WGM12 | 1<<WGM13 | 1<< CS11 | 1<<CS10;
	
	ICR1=4999;
	
	while(1)
	{
		//transmit('A');
		//_delay_ms(2000);

	}
}

ISR(USART_RXC_vect)
{
	
	int i=0,j=0,a[5];
	
	reievebyte=UDR;
	transmit(reievebyte);
	if(reievebyte==49)
	{
		OCR1A=(int)(ICR1-((ICR1+1)*SERVO_0/20)-1);
		_delay_ms(1000);
	}
	
	if(reievebyte==50)
	{
		OCR1A=(int)(ICR1-((ICR1+1)*SERVO_90/20)-1);
		_delay_ms(1000);
		
	}
	if(reievebyte==51)
	{
		OCR1A=(int)((ICR1+1)*SERVO_180/20)-1;
		_delay_ms(1000);
	}
}