/*
 * cbfifo.h
 *
 *  Created on: 13-Dec-2023
 *      Author: Shruthi Thallapally
 *   Reference: Used the code from previous assignments
 */

#ifndef CBFIFO_H_
#define CBFIFO_H_

#include <stdio.h>
#include <stdint.h>  // for size_t

#define SIZE 127

//data structure to define circular fifo components
typedef struct
{
	uint8_t data[SIZE];
	unsigned int first;
	unsigned int last;
	unsigned int length;
} volatile cbfifo_t;

/*
 * Function to initialize circular buffer
 * Parameters- buf of type cbfifo_t
 * Return type- void
 */
void cbfifo_init(cbfifo_t *buffer);

/*
 * Function to check if fifo is empty
 * Parameters- buf of type cbfifo_t
 * Return type- integer
 */
int cbfifo_empty(cbfifo_t *buffer);

/*
 * Function to check if fifo is full
 * Parameters- buf of type cbfifo_t
 * Return type- integer
 */
int cbfifo_full(cbfifo_t *buffer);

/*
 * Enqueues data onto the FIFO, up to the limit of the available FIFO
 * capacity.
 *
 * Parameters:
 *   buf      Pointer to the data
 *   nbyte    Max number of bytes to enqueue
 *
 * Returns:
 *   The number of bytes actually enqueued, which could be 0. In case
 * of an error, returns -1.
 */
size_t cbfifo_enqueue(cbfifo_t *buffer, size_t nbyte);


/*
 * Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * Parameters:
 *   buf      Destination for the dequeued data
 *   nbyte    Bytes of data requested
 *
 * Returns:
 *   The number of bytes actually copied, which will be between 0 and
 * nbyte.
 *
 *
 */
size_t cbfifo_dequeue(cbfifo_t *buffer);


/*
 * Returns the number of bytes currently on the FIFO.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   Number of bytes currently available to be dequeued from the FIFO
 */
size_t cbfifo_length(cbfifo_t *buffer);


#endif /* CBFIFO_H_ */
