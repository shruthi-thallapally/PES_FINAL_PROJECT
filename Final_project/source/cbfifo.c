/*
 * cbfifo.c
 *
 *  Created on: 13-Dec-2023
 *      Author: Shruthi Thallapally
 *   Reference: Used the code from previous assignments
 */

#include <stdio.h>
#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include "sysclock.h"
#include "cbfifo.h"
#include "uart0.h"

/* Function: cbfifo_init
 Description: Initializes the circular buffer FIFO.
 Parameters:
   - buf: Pointer to the circular buffer FIFO structure.
 Returns: None  */

void cbfifo_init(cbfifo_t *buffer)
{
	unsigned int i;
	for(i=0; i<SIZE; i++)
		buffer->data[i] = 0;			//empty data in buffer if any
	buffer->first=0;					//start at first position
	buffer->last=0;					//keep the end at first position
	buffer->length=0;					//reset buffer length
}

/* Function: fifo_empty
 Description: Checks if the circular buffer FIFO is empty.
 Parameters:
   - buf: Pointer to the circular buffer FIFO structure.
 Returns: 1 if the FIFO is empty, 0 otherwise.  */

int cbfifo_empty(cbfifo_t *buffer)
{
	return buffer->length == 0;
}

/* Function: fifo_full
 Description: Checks if the circular buffer FIFO is full.
 Parameters:
   - buf: Pointer to the circular buffer FIFO structure.
 Returns: 1 if the FIFO is full, 0 otherwise.   */

int cbfifo_full(cbfifo_t *buffer)
{
	return buffer->length == SIZE;
}

/* Function: cbfifo_enqueue
 Description: Enqueues a byte into the circular buffer FIFO.
 Parameters:
   - buf: Pointer to the circular buffer FIFO structure.
   - nbyte: Byte to be enqueued.
 Returns: 1 on success, 0 on failure.  */

size_t cbfifo_enqueue(cbfifo_t *buffer, size_t nbyte)
{

	uint32_t masking_state;

	if(!cbfifo_full(buffer)) 					//check if fifo is full
	{
		buffer->data[buffer->last++] = nbyte;				//store data in buffer
		buffer->last %= SIZE;

		masking_state = __get_PRIMASK();			//save current masking state

		__disable_irq();							//disable interrupt

		buffer->length++;								//increment length of buffer

		__set_PRIMASK(masking_state);				//restore interrupt masking state
		return 1;									//success
	}
	else
		return 0;									//failure
}

/* Function: cbfifo_dequeue
 Description: Dequeues a byte from the circular buffer FIFO.
 Parameters:
   - buf: Pointer to the circular buffer FIFO structure.
 Returns: The dequeued byte.  */

size_t cbfifo_dequeue(cbfifo_t *buffer)
{

	uint32_t masking_state;
    size_t newbyte=0;

    if(!cbfifo_empty(buffer)) 				//check if fifo is empty
    {
    	newbyte = buffer->data[buffer->first];			//store the first byte
    	buffer->data[buffer->first++] = '_';				//empty unused entries
    	buffer->first %= SIZE;

    	masking_state = __get_PRIMASK();			//save current masking state

    	__disable_irq();							//disable interrupt

    	buffer->length--;								//decrement length of buffer

    	__set_PRIMASK(masking_state);				//restore masking state
    }
	return newbyte;
}

/* Function: cbfifo_length
 Description: Returns the current length of the circular buffer FIFO.
 Parameters:
   - buf: Pointer to the circular buffer FIFO structure.
 Returns: Current length of the FIFO.  */

size_t cbfifo_length(cbfifo_t *buffer)
{

	 return buffer->length;
}


