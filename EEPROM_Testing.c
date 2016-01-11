#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR


#define address1 0x02
#define address2 0x03
#define data1 2
#define data2 5


//#define read_eeprom_word(address) eeprom_read_byte (const uint16_t*)address)
//#define write_eeprom_word(address,value) eeprom_write_byte ((uint16_t*)address,(uint16_t)value)

#define eewe EEWE
#define eemwe EEMWE
#define eere EERE

volatile unsigned char recievebyte;

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


void EEPROM_write(unsigned int uiAddress, unsigned char ucData)

{
	
	/* Wait for completion of previous write */
	
	while(EECR & (1<<eewe));
	
	/* Set up address and data registers */
	
	EEARH = uiAddress>>8;
	EEARL = uiAddress;
	EEDR = ucData;
	
	/* Write logical one to EEMWE */
	
	EECR |= (1<<eemwe);
	
	/* Start eeprom write by setting EEWE */
	
	EECR |= (1<<eewe);
	
}

unsigned char EEPROM_read(unsigned int uiAddress)

{
	
	/* Wait for completion of previous write */
	
	while(EECR & (1<<eewe));
	
	/* Set up address register */
	
	EEAR = uiAddress;
	
	/* Start eeprom read by writing EERE */
	
	EECR |= (1<<eere);
	
	/* Return data from data register */
	
	return EEDR;
	
}
int main(void)
{
	
	uart_init();
	
	sei();
	EEPROM_write(address1,data1);
	EEPROM_write(address2,data2);
	DDRB=0xff;
	//PORTB=0xff;
	//uint8_t check1,check2,port=0;
	while(1)
    {
		/*if(port==0)
		port=15;
		
        check1=EEPROM_read(address1);
		check2=EEPROM_read(address2);
		if(port==check1 || port==check2)
		{transmit(port+48);_delay_ms(100);}
			port--;
		*/
    }
}

ISR (USART_RXC_vect)
{
	recievebyte=UDR;
	
	if(recievebyte==50)
	{
		transmit('U');
		transmit('N');
		transmit('L');
		transmit('O');
		transmit('C');
		transmit('K');
		transmit('E');
		transmit('D');
		transmit(13);
	}
	
	else
	{
		transmit('L');
		transmit('O');
		transmit('C');
		transmit('K');
		transmit('E');
		transmit('D');
		transmit(13);
	}
}