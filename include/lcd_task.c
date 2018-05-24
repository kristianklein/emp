/*
 * lcd_task.c
 *
 *  Created on: 24. maj 2018
 *      Author: Anders Sørensen
 */

/***************************** Included files ********************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "FreeRTOS.h"
#include "lcd.h"
#include "task.h"
#include "queue.h"

/***************************** Defines ***************************************/
#define WAIT_FOR_POWER_ON           3       //Important! if the time slice size is changed, then change this number



/***************************** Constants *************************************/
/***************************** Variables *************************************/

/***************************** Helper Functions decelerations ****************/

/***************************** Functions *************************************/
void lcd_task(void *pvParameters)
{
lcd_init();

lcd_write('A');
lcd_write('N');
lcd_write('D');
lcd_write('E');
lcd_write('R');
lcd_write('S');


while(1)
{

    GPIO_PORTF_DATA_R |= 0xE;
}

return;
}




