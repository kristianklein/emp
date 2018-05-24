/*
 * keypad_task.c
 *
 *  Created on: 24. maj 2018
 *      Author: Anders Sørensen
 */

/***************************** Included files ********************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "FreeRTOS.h"
#include "keypad.h"
#include "task.h"
#include "queue.h"

/***************************** Defines *******************************/
#define Q_FULL_TICKS_WAIT 5
#define TASK_DELAY_TIME 5
/***************************** Constants *******************************/
/***************************** Variables *******************************/
/***************************** Functions *******************************/
void keypad_task(void *pvParameters)
{
    extern xQueueHandle xQueueKeypad;
    keypad_init();
    INT8U key_press = 0;

    while(1)
    {
        key_press = keypad_read();
        xQueueSendToBack(xQueueKeypad,(void*) &key_press, Q_FULL_TICKS_WAIT);
        vTaskDelay(TASK_DELAY_TIME);

    }
    return;
}

