/*
 * GccApplication20.c
 *
 * Created: 27-02-2015 16:54:46
 *  Author: Aashay
 */ 


#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define BAUD 9600
#define UBRR F_CPU/16/BAUD-1

void USART_Init( uint16_t ubrr)

{
	
	UBRRH = (uint8_t)(ubrr>>8);
	UBRRL = (uint8_t)ubrr;
	
	UCSRB = (1<<RXEN)|(1<<TXEN);
    UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ0)|(1<<UCSZ1);
	
	}
	
 char USART_Receive( void )
	
{
		
		while ( !(UCSRA & (1<<RXC)) );
		
	return UDR ; }
	

int main(void)
{
	char received;
	int converted;
	DDRD= 0b00100000;
	TCCR1A |= (1<<COM1A1)|(1<<WGM11)|(1<<WGM10);
	TCCR1B |= (1<<WGM12)|(1<<CS10)|(1<<CS11);
	ICR1= 4999;

	USART_Init(UBRR);
	
    while(1)
    {
_delay_ms(500);
received_h = USART_Receive();
converted=100*(int)(received_h-48);
flush = USART_Receive();
received_t = USART_Receive();
converted+=10*(int)(received_t-48);
flush = USART_Receive();
received_o = USART_Receive();
converted+=(int)(received_o-48);
flush = USART_Receive();
OCR1A= (int) (0.55 + 1.85*converted/180)/20*4999;
    }
return 0;}


