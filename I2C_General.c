#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#define TRUE 1
#define FALSE 0
#define addr 0x20
#include <avr/interrupt.h>

//#include "General.h"
//#include "TWI_Master.h"

/****************************************************************************
  TWI State codes
****************************************************************************/
// General TWI Master staus codes                      
#define TWI_START                    0x08  // START has been transmitted  
#define TWI_REP_START                0x10  // Repeated START has been transmitted
#define TWI_ARB_LOST                0x38  // Arbitration lost

// TWI Master Transmitter staus codes                      
#define TWI_MTX_ADR_ACK                0x18  // SLA+W has been tramsmitted and ACK received
#define TWI_MTX_ADR_NACK            0x20  // SLA+W has been tramsmitted and NACK received 
#define TWI_MTX_DATA_ACK            0x28  // Data byte has been tramsmitted and ACK received
#define TWI_MTX_DATA_NACK            0x30  // Data byte has been tramsmitted and NACK received 

// TWI Master Receiver staus codes  
#define TWI_MRX_ADR_ACK                0x40  // SLA+R has been tramsmitted and ACK received
#define TWI_MRX_ADR_NACK            0x48  // SLA+R has been tramsmitted and NACK received
#define TWI_MRX_DATA_ACK            0x50  // Data byte has been received and ACK tramsmitted
#define TWI_MRX_DATA_NACK            0x58  // Data byte has been received and NACK tramsmitted

// TWI Slave Transmitter staus codes
#define TWI_STX_ADR_ACK                0xA8  // Own SLA+R has been received; ACK has been returned
#define TWI_STX_ADR_ACK_M_ARB_LOST    0xB0  // Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
#define TWI_STX_DATA_ACK            0xB8  // Data byte in TWDR has been transmitted; ACK has been received
#define TWI_STX_DATA_NACK            0xC0  // Data byte in TWDR has been transmitted; NOT ACK has been received
#define TWI_STX_DATA_ACK_LAST_BYTE    0xC8  // Last data byte in TWDR has been transmitted (TWEA = “0”); ACK has been received

// TWI Slave Receiver staus codes
#define TWI_SRX_ADR_ACK                0x60  // Own SLA+W has been received ACK has been returned
#define TWI_SRX_ADR_ACK_M_ARB_LOST    0x68  // Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
#define TWI_SRX_GEN_ACK                0x70  // General call address has been received; ACK has been returned
#define TWI_SRX_GEN_ACK_M_ARB_LOST    0x78  // Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_ACK        0x80  // Previously addressed with own SLA+W; data has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_NACK        0x88  // Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
#define TWI_SRX_GEN_DATA_ACK        0x90  // Previously addressed with general call; data has been received; ACK has been returned
#define TWI_SRX_GEN_DATA_NACK        0x98  // Previously addressed with general call; data has been received; NOT ACK has been returned
#define TWI_SRX_STOP_RESTART        0xA0  // A STOP condition or repeated START condition has been received while still addressed as Slave

// TWI Miscellaneous status codes
#define TWI_NO_STATE                0xF8  // No relevant state information available; TWINT = “0”
#define TWI_BUS_ERROR                0x00  // Bus error due to an illegal START or STOP condition

#define TWIM_READ    1
#define TWIM_WRITE   0

	
unsigned char SLA_recieve_Data (void)
{
	unsigned char datavariable;
	datavariable=TWDR;
	return datavariable;
}


	
void SLA_Recieve_Data_ACKorNACK (uint8_t TYPE)//1-ACK // 0-NACK
{	
	uint8_t twst;
	
	twst = TWSR & 0xF8;
	
	if (TYPE) TWCR|= (1<<TWEN)|(1<<TWEA); //TWEA NOT FOR NACK
	if (!TYPE) TWCR|= (1<<TWEN);
	 while(!(TWCR & (1<<TWINT)));//SET BIT
	
	if(TYPE)
	{
		while (twst != TWI_SRX_ADR_DATA_ACK);
	}
	
	if(!TYPE)
	{
	while((twst != TWI_SRX_ADR_DATA_NACK));
	}
}

void SLA_Recieve_Adress_ACK(void)
{
	uint8_t twst,address;
	address=TWDR;
	if(((address>>1)<<1)== addr)
	{TWCR|= (1<<TWEN)|(1<<TWEA);
		while(!(TWCR & (1<<TWINT)));//SET BIT
		
			twst = TWSR & 0xF8;
			while( (twst != TWI_SRX_ADR_ACK) );
		}
	else {PORTB=0b01011101; }
	}
	
	void SLA_Init (void)
	{
		/*
		** Set TWI bitrate
		** If bitrate is too high, then error return
		TWBR = ((F_CPU/TWI_Bitrate)-16)/2;*/
		TWSR=(0<<TWPS1)|(0<<TWPS0);
		TWAR=0x20;
	}
	
	int main ()
	{
		DDRB=0xff;
		unsigned char data;
		
		SLA_Init();
		while(1)
		{
			SLA_Recieve_Adress_ACK();
				data=SLA_recieve_Data();
				PORTB=data;
				SLA_Recieve_Data_ACKorNACK(1);
		}
	}