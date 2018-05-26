/*
 * digiswitch.c
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

/***************************** Defines ***************************************/
/***************************** Constants *************************************/
/***************************** Variables *************************************/
INT8U AB_pre;
/***************************** Helper decelerations **************************/
INT8U digiA_read();
INT8U digiB_read();
INT8U digiP2_read();
INT8U AB_read();



/***************************** Functions *************************************/
void digi_init()
{

    //Enable clock for port A
     SYSCTL_RCGC2_R |= 0x01;

     //Set the direction (0=input and 1=output)
     GPIO_PORTA_DIR_R &= ~(0xE0);        //Digi -> PORTA 5-7

     //Enable the GPIO pins
     GPIO_PORTA_DEN_R |= 0xE0;

     //Enable internal pull-up for the high nibble
     GPIO_PORTA_PUR_R &= ~0xE0;
     AB_pre = AB_read();

    return;
}


INT8U digi_turn()
{
    INT8U AB, event = 0, YY;

    AB = AB_read();

    if(!(AB == AB_pre))
    {
        YY = AB ^ AB_pre;
        if((AB & 0x2) == (AB & 0x1))
        {
            if(YY == 0x1)
            {
                event = 1;
            }
            else if(YY == 0x2)
            {
                event  = 2;
            }
        }
        else
        {
            if(YY == 0x1)
            {
                event = 2;
            }
            else if(YY == 0x2)
            {
                event  = 1;
            }
        }

        AB_pre = AB;
    }

    return(event);
}


INT8U digi_press()
{
    static INT8U digi_press_state = 0;
    INT8U event = 0;
    if(!digi_press_state)
    {

        if(!digiP2_read())
        {
            rgb_set(1,0,0);
            event = 1;
            digi_press_state = 1;
        }
    }
    else if(digi_press_state)
    {
        if(!digiP2_read())
        {
            digi_press_state = 0;
        }
    }

    return(event);

}
/***************************** Helper Functions ******************************/

INT8U digiA_read()
{
    return(GPIO_PORTA_DATA_R & 0x20);
}


INT8U digiB_read()
{
    return(GPIO_PORTA_DATA_R & 0x40);
}

INT8U digiP2_read()
{
    return(GPIO_PORTA_DATA_R & 0x80);
}

INT8U AB_read()
{
    INT8U AB = 0;

    if(digiA_read())
    {
        AB |= 0x2;
    }
    if(digiB_read())
    {
        AB |= 0x1;
    }

    return(AB);
}



