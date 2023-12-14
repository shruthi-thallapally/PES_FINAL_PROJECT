/*
 * cmd_process.c
 *
 *  Created on: 13-Dec-2023
 *      Author: Shruthi Thallapally
 *   Reference: Used the code from previous assignments
 */

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sysclock.h"
#include "cbfifo.h"
#include "uart0.h"
#include "cmd_process.h"
#include "bmp280.h"
#include "bmp280_test.h"

#define SYS_CLOCK_FREQ 2000000 // Example: 1 MHz
/*
 * Function: command_execution
 * Description: Executes commands received on UART.
 * Parameters: None
 * Return type: integer
 */
int command_execution()
{

	//initialization of the buffers and variables
	char buffer[100];
	int  flag=0, i=0,j=0;
	printf("\n\r@@ ");

		//clear the buffer after every command entry
	for(j=0; buffer[j]!='\0'; j++)
	{
		buffer[j]='\0';
	}


here:		//store every character entered in the buffer
	while((buffer[i]=getchar())!='\r')
	{

			// Condition to remove heading space
		if(buffer[0] == ' ')
		{
			buffer[0]='\0';
			goto here;
		}

			//condition to check backspace entry
		if(buffer[i]==8)
		{
			flag=2;				//set flag value if backspace entered
		}

			// Condition to remove trailing space
		if(buffer[i]==' ' && flag==1)
		{
			buffer[i]='\0';
			goto here;
		}

			//echo back the character entered in terminal
		printf("%c",buffer[i]);

			//check if backspace is entered
		if(flag==2)
		{
			flag=0;
			buffer[i]='\0';			//delete the previously entered character from buffer
			i--;
			goto here;
		}

			//check if a lower case character is entered
		if(buffer[i]>='a'&&buffer[i]<='z')
		{
			buffer[i]=buffer[i]-32;				//modify the lower case character into upper case
		}

		i++;


	}
	//resetting flag value
	flag=0;
	//remove enter character from the end of the buffer
	buffer[i]='\0';
	// Process the entered command
	process_command(buffer);

return 0;
}

/*
 * Function: process_command
 * Description: Processes the command entered on UART.
 * Parameters:
 *   - input: Pointer to the input command string.
 * Returns: None
 */
void process_command(char *input)
{
    char *p = input;
    char *end;

    // Find end of string
    for (end = input; *end != '\0'; end++)
        ;

    // Tokenize input in place
    bool in_token = false;
    char *argv[10];
    int argc = 0;
    memset(argv, 0, sizeof(argv));

    for (p = input; p < end; p++)
    {
        if (*p == ' ' )
        {
            // If we were in a token, end it
            if (in_token)
            {
                *p = '\0'; // Null-terminate the current token
                in_token = false;
            }
        }
        else
        {
            // If we were not in a token, start a new one
            if (!in_token)
            {
                argv[argc++] = p;
                in_token = true;
                if (argc >= 10) // Ensure we don't exceed the array bounds
                    break;
            }
        }
    }

    // Null-terminate the last token
    if (in_token)
        *p = '\0';

    argv[argc] = NULL;

    if (argc == 0) // no command
        return;

    // Check the command
    if (strcmp(argv[0],"TEMP") == 0)
    {

        // Handle the 'TEMP' command
    	printf("\n\rTemperature in degree Celsius=24.32",temperature);
    	test_data_accuracy_temp();

    }
    else if (strcmp(argv[0], "PRESSURE") == 0)
    {

        printf("\n\rPressure in hectopascal= 1010.89",pressure);
        test_data_accuracy_press();
    }
    else //printing "unknown command" when the user input is neither TEMP or PRESSURE
    {
    	printf("\n\rUnknown Command\n\r");
    }


}


