#include <stdint.h>
#include "lpc17xx.h"
#include "setup.h"

int no_of_bytes = 0;
int first_byte = 0;
int second_byte = 0;
int first_byte_location = 0;
int second_byte_location = 0;


extern uint8_t index_of_modbus_command;
extern uint8_t modbus_command[];
extern void main_give_semaphore_uart0( void );


static uint32_t uart_clock_calc( void )
{
	uint32_t pclk_val, temp;
	pclk_val = (( LPC_SC->PCLKSEL0 )>>6 ) & 0x03;
	switch( pclk_val )
	{
		case 0x00:
			temp = SystemCoreClock/4;
		break;
		case 0x01:
			temp = SystemCoreClock;
		break;
		case 0x02:
			temp = SystemCoreClock/2;
		break;
		case 0x03:
			temp = SystemCoreClock/8;
		break;
	}
	return temp;
}

void uart0_init( uint32_t baudrate, uint32_t lcr, enum IRQ_CAT irq)
{
	uint32_t dlm_dll_combined;
	dlm_dll_combined = (uart_clock_calc()/(16*baudrate));
		
		// UART0 Specific pinsel register values
		LPC_PINCON->PINSEL0 &= ~((1<<5)|(1<<4));
		LPC_PINCON->PINSEL0 |= (1<<4);
	
		LPC_PINCON->PINSEL0 &= ~((1<<7)|(1<<6));
		LPC_PINCON->PINSEL0 |= (1<<6);
	
		LPC_UART0->LCR = lcr; //0x83; // DLAB on, 1 stop bit, no pairity and 8 bit character
//		LPC_UART0->DLL = 2; //(dlm_dll_combined & 0xFF);	//2 for 2400
//		LPC_UART0->DLM = 2; //((dlm_dll_combined >> 8) & 0xFF);	//2 for 2400
//		LPC_UART0->FDR = 0xF4;
	
	
		LPC_UART0->DLL = (dlm_dll_combined & 0xFF);
		LPC_UART0->DLM = ((dlm_dll_combined >> 8) & 0xFF);
		LPC_UART0->LCR &= (~(1<<DLAB));	// clearing the DLAB bit 0x80
	
	// let us enable interrupt also
	if(irq)
	{
		NVIC_EnableIRQ(UART0_IRQn);
		LPC_UART0->IER = (1<<RBR_IE)|(1<<RX_LINE_IE);	// interrupt enabled on Recieve Data Available
	}
	//configure FCR(FIFO Control Register)
	LPC_UART0->FCR = (1<<FIFOEN)|(1<<RXFIFO_CLR)|(1<<TXFIFO_CLR);	
}



void UART0_IRQHandler( void )
{
	uint8_t a;
	if(LPC_UART0->LSR&0x01)
	{
		modbus_command[index_of_modbus_command++] = LPC_UART0->RBR;
	}
	
	if(index_of_modbus_command == 100)
	{
		index_of_modbus_command=0;
	}
	
	if(modbus_command[index_of_modbus_command-1] == SLAVE_ADDRESS)
	{
		first_byte_location = index_of_modbus_command-1;
	}
	
	if(modbus_command[index_of_modbus_command-1] == FUNCTION_NUMBER)
	{
		second_byte_location = index_of_modbus_command-1;
	}
	
	if( first_byte_location == (second_byte_location-1))
	{
		if(modbus_command[first_byte_location+2] == 0x00)
		{
			if(modbus_command[first_byte_location+3] == 0x00)
			{
				if(modbus_command[first_byte_location+4] == 0x00)
				{
					if(modbus_command[first_byte_location+5] == 0x10)
					{
						main_give_semaphore_uart0();
					}
				}
			}
		}
	}
}

void uart0_tx( uint8_t data )
{ 
	while(!(LPC_UART0->LSR&0x20));
	LPC_UART0->THR = data;
}

void uart0_tx_string( uint8_t *str )
{
	while(*str)
	{
		uart0_tx( *str );
		str++;
	}
}

void uart0_tx_byte( uint8_t *byte, uint8_t length )
{
	while(length--)
	{
		uart0_tx((uint8_t) *byte );
		byte++;
	}
}

