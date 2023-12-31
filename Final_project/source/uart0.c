/*
 * uart0.c
 *
 *  Created on: 13-Dec-2023
 *      Author: Shruthi thallapally
 *   Reference: Used the code from previous assignments
 */

#include <mkl25z4.h>
#include "sysclock.h"
#include "uart0.h"
#include "cbfifo.h"

#define UART_OVERSAMPLE_RATE 16
#define BAUD_RATE 38400
#define DATA_BIT 9
#define STOP_BIT 1
#define PARITY_BIT 1

//define transmit and receive buffers
cbfifo_t TxQ;
cbfifo_t RxQ;

/*
 * Function: __sys_readc
 * Description: Custom read function to read a character from the UART receive buffer.
 * Returns: Integer representing the read character.
 */
int __sys_readc(void) {
	uint8_t c;
	while(cbfifo_length(&RxQ)==0);					//blocking receive and wait for characters to arrive
	c = cbfifo_dequeue(&RxQ);
	return c;
}

/*
 * Function: __sys_write
 * Description: Custom write function to write a string to the UART transmit buffer.
 * Parameters:
 *   - handle: File handle (1 for stdout).
 *   - buf: Pointer to the buffer containing the string to be written.
 *   - size: Size of the string.
 * Returns: 0 if successful, -1 otherwise.
 */
int __sys_write(int handle, char *buf, int size) {
	char *bp;
	switch(handle) {
		case 1:
			bp=buf;									//enqueue string
			while(*bp != '\0') {					//copy characters upto null terminator
				while(cbfifo_full(&TxQ));				//wait for space to open up
				cbfifo_enqueue(&TxQ, *bp);
				bp++;
			}
		    if(!(UART0->C2 & UART0_C2_TIE_MASK)) {	//start transmitter if it isn't already running
		    	UART0->C2 |= UART0_C2_TIE(1);
		    }
	   	 return 0;

	     default:       return -1;
	}
}


/*
 * Function: init_UART0
 * Description: Initializes the UART0 module with specified configuration parameters.
 * Returns: None
 */
void init_UART0() {

	uint16_t sbr;

	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;				//enable clock gating for UART0
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;				//enable clock gating for PORTA

	UART0->C2 |= UART0_C2_TE(0)						//disable the transmitter
					| UART0_C2_RE(0);				//disable the receiver

	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);			//set UART clock

	//set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);	//RX
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2);	//TX

	//set baud rate and over sampling rate
	sbr = (uint16_t)((SYSCLOCK_FREQUENCY)/(BAUD_RATE * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	UART0->BDH |= UART0_BDH_RXEDGIE(0)				//disable interrupt for RX active edge
					| UART0_BDH_SBNS(STOP_BIT)				//select two stop bits
					| UART0_BDH_LBKDIE(0);			//LIN break detect

	UART0->C1 = UART0_C1_LOOPS(0)					//disable loopback mode
					| UART0_C1_M(DATA_BIT)					//use 8 bit data mode
					| UART0_C1_PE(PARITY_BIT)|UART0_C1_PT(PARITY_BIT);				//no parity

	UART0->C3 = UART0_C3_TXINV(0)					//disable transmit data inversion
					| UART0_C3_ORIE(1)				//enable interrupt for errors
					| UART0_C3_NEIE(1)
					| UART0_C3_FEIE(1)
					| UART0_C3_PEIE(1);

	UART0->S1 = UART0_S1_OR(1)						//clear error flags
					| UART0_S1_NF(1)
					| UART0_S1_FE(1)
					| UART0_S1_PF(1);

	UART0->S2 = UART0_S2_MSBF(0)					//send LSB first
					| UART0_S2_RXINV(0);			//disable receive data inversion

	UART0->C2 |= UART0_C2_TE(1)						//enable UART transmitter
					| UART0_C2_RE(1)				//enable UART receiver
					| UART0_C2_RIE(1);				//enable UART interrupt

	cbfifo_init(&TxQ);					//initialize transmit fifo
	cbfifo_init(&RxQ);					//initialize receive fifo
}

/*
 * Function: UART0_IRQHandler
 * Description: Interrupt handler for UART0, handles errors, reception, and transmission.
 * Returns: None
 */
void UART0_IRQHandler(void) {
	uint8_t ch;

	 // Handle error conditions
	if(UART0->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK)) {
		UART0->S1 |= UART0_S1_OR_MASK 				//clear error flags
						| UART0_S1_NF_MASK
						| UART0_S1_FE_MASK
						| UART0_S1_PF_MASK;
		ch = UART0->D;								//read data register
	}
	// Handle receive interrupt
	if(UART0->S1 & UART0_S1_RDRF_MASK) {
		ch = UART0->D;								//receive a character
		if(!cbfifo_full(&RxQ)) {
			cbfifo_enqueue(&RxQ, ch);
		}
		else {
			// Error handling for full receive queue
		}
	}
	// Handle transmit interrupt
	if((UART0->C2 & UART0_C2_TIE_MASK) && 			//transitter interrupt enabled
			(UART0->S1 & UART0_S1_TDRE_MASK)) {		//transmit buffer empty
		if(!cbfifo_empty(&TxQ)) {
			UART0->D = cbfifo_dequeue(&TxQ);
		}
		else {
			// Queue is empty, so disable transmitter interrupt
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}


