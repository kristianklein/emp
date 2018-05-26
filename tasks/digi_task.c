/*
 * digi_task.c
 *
 *  Created on: 25. maj 2018
 *      Author: Anders S�rensen
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
#include "digiswitch.h"


/***************************** Defines ***************************************/
#define Q_FULL_TICKS_WAIT           1
#define WAIT_10_MS                  2
/***************************** Constants *************************************/
/***************************** Variables *************************************/

/***************************** Functions *************************************/
void digi_task(void *pvParameters)
{
    digi_init();

    INT8U turn, press;
    extern xQueueHandle xQueueDigi_switch;

    while(1)
    {
        turn = digi_turn();
        press = digi_press();

        if(turn)
        {
            xQueueSendToBack(xQueueDigi_switch,(void*) &turn, Q_FULL_TICKS_WAIT);
        }
        if(press)
        {
            xQueueSendToBack(xQueueDigi_switch,(void*) &press, Q_FULL_TICKS_WAIT);
        }
    }

    return;
}


