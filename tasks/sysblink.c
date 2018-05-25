/*
 * sysblink.c
 *
 *  Created on: 25. maj 2018
 *      Author: Anders Sørensen
 */


/***************************** Included files ********************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "FreeRTOS.h"
#include "task.h"

/***************************** Defines *******************************/
#define WAIT_ONE_SEK            200
/***************************** Variables *******************************/

/***************************** Helper decelerations **************************/
void init_sysblink();


/***************************** Functions *******************************/
void sysblink(void *pvParameters)
{
    init_sysblink();

    while(1)
    {
        GPIO_PORTD_DATA_R ^= 0x40;
        vTaskDelay(WAIT_ONE_SEK);
    }
    return;
}


void init_sysblink()
{
    SYSCTL_RCGCGPIO_R |= 0x8;
    GPIO_PORTD_DIR_R |= 0x40;        //Set the direction (0=input and 1=output)
    GPIO_PORTD_DEN_R |= 0x40;           //Enable the GPIO pins
    GPIO_PORTD_PUR_R &= ~0x40;         //Enable internal pull-up
    return;
}


