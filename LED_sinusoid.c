#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

int main(void)
{
	uint16_t i=0;
	DDRD=0xff;
	TCCR1A|=0<<WGM11 |1<<COM1A1 | 1<<COM1A0;
	TCCR1B|=0<<WGM12 | 1<<WGM13 | 1<< CS11 | 1<<CS10;
	
	ICR1=2499;
	while (1)
	{
		while(i<2500)
			{
				
				OCR1A=2499*sin(1.57*i/2499);
				i++;
				if(i==2499)
				{
					while(i>0)
					{
						OCR1A=2499*sin(1.57*i/2499);
						i--;
					}
				}
			}
		i=0;
	}
}
