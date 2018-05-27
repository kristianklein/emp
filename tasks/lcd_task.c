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
#include "rgb.h"

/***************************** Defines ***************************************/
#define WAIT_FOR_POWER_ON           3       //Important! if the time slice size is changed, then change this number


/***************************** Constants *************************************/
/***************************** Variables *************************************/

/***************************** Helper Functions decelerations ****************/

/***************************** Functions *************************************/
void lcd_task(void *pvParameters)
{
    extern xQueueHandle xQueueLCD;
    INT8U char_value;
    lcd_init();
    lcd_clear();

    while(1)
    {
        if(xQueueReceive(xQueueLCD,(void *) &char_value, 5))
        {

            if(char_value == 0x0A)
            {
                lcd_newline();
            }
            else if(char_value  == 0x00)
            {
                lcd_clear();
            }
            else
            {
                lcd_write(char_value);
            }
        }
    }

    return;
}








