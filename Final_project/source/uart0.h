/*
 * uart0.h
 *
 *  Created on: 13-Dec-2023
 *      Author: Shruthi thallapally
 *   Reference: Used the code from previous assignments
 */

#ifndef UART0_H_
#define UART0_H_


/*
 * Inbuilt function to write specified bytes.
 * This function ties UART code with standard library
 * functions like printf and putchar
 * Parameters- handle - to write to stdout or stderr
 * 			   buf - the character pointer which is to be written
 * 			   size - size of buf
 */
int __sys_write(int handle, char *buf, int size);

/*
 * Inbuilt function to read one character from UART
 * Parameters- none
 * Return type- integer
 */
int __sys_readc(void);

/*
 * function to initialize UART communication
 * Parameters- none
 * Return type- void
 */
void init_UART0();

/*
 * UART interrupt handler function
 * Parameters- none
 * Return type- void
 */
void UART0_IRQHandler(void);



#endif /* UART0_H_ */
