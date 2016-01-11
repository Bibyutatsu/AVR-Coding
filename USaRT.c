#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define SERVO_0 149
#define SERVO_90 374
#define SERVO_180 599
#define SE_120 449
#define SE_100 399
#define SE_30 224
#define SE_60 299
int main(void)
{
	DDRD=0xff;
	//uint16_t i=0;
	TCCR1A|=1<<WGM11 |1<<COM1A1 | 1<<COM1A0;
	TCCR1B|=1<<WGM12 | 1<<WGM13 | 1<< CS11 | 1<<CS10;
	//TIMSK |= 1<<OCIE1A;
	
	//sei();
	
	ICR1=4999;
	
	
	while (1)
	{
		OCR1A=(int)(ICR1-SERVO_0);
		_delay_ms(2000);
		
		OCR1A=(int)(ICR1-SERVO_90);
		_delay_ms(2000);
		
		OCR1A=(int)(ICR1-SERVO_180);
		_delay_ms(2000);
		
		OCR1A=(int)(ICR1-SE_120);
		_delay_ms(2000);
		
		OCR1A=(int)(ICR1-SE_100);
		_delay_ms(2000);
		
		OCR1A=(int)(ICR1-SE_30);
		_delay_ms(2000);
		
}
}
/*ISR (TIMER1_COMPA_vect)
{
	PORTB=0xff;
}*/