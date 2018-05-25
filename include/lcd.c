/*
 * lcd.c
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
#define Q_FULL_TICKS_WAIT           5
#define TASK_DELAY_TIME             5

//LCD delay
#define DELAY_OUTER_LOOP            255
#define DELAY_INNER_LOOP            50
#define WAIT_FOR_LCD_READY          1       //Important! if the time slice is size changed, then change this number

//PIN defines
#define PIN_E                       0x08




/***************************** Constants *************************************/
/***************************** Variables *************************************/

/***************************** Helper Functions decelerations ****************/
void lcd_delay();
void flash_pin_E();
void clear_lcd_data_pins();
void lcd_port_setup();
/***************************** Functions *************************************/
void lcd_init()
{
    //Port D (RS - PD2 and E-PD3)
    //Port C (D4-D7)
    //High nibble first

    //Initialize the ports
    lcd_port_setup();

    //Chance the LCD-to command mode
    GPIO_PORTD_DATA_R &= ~(0x04);       //Clear RS to make a command

    //Set Interface data to 4 bit.
    lcd_write(0x22);

    //Set Interface data to 4 bit, number of lines to 2 lines and charector fond to 5x8
    lcd_write(0x28);

    //Clear the display
    lcd_write(0x01);

    //Set the direction
    lcd_write(0x06);

    //Turn on the display
    lcd_write(0x0C);

    //Make ready to write
    GPIO_PORTD_DATA_R |= 0x04;       //Set RS to write
    return;
}


void lcd_write(INT8U char_value)
{
    INT8U temp = char_value & 0xF0;

    clear_lcd_data_pins();
    GPIO_PORTC_DATA_R |= temp;
    flash_pin_E();
    lcd_delay();                           //Delay the 40us to give the display time to

     temp = char_value << 4;               //Move low nibble to high

     clear_lcd_data_pins();
     GPIO_PORTC_DATA_R |= temp;             //Set the 4-bit command
     flash_pin_E();
     lcd_delay();                          //Delay the 40us to give the display time to

    return;
}


void lcd_change_line(INT8U linenr)
{
    clear_lcd_data_pins();
    if(linenr == 1)
    {





    }
    return;
}

void lcd_clear()
{
    GPIO_PORTD_DATA_R &= ~(0x04);      //Clear RS to make a command
    lcd_write(0x01);
    GPIO_PORTD_DATA_R |= 0x04;         //set RS to send a data
    return;
}


void lcd_return_home()
{
    GPIO_PORTD_DATA_R &= ~(0x04);        //Clear RS to make a command
    lcd_write(0x02);
    GPIO_PORTD_DATA_R |= 0x04;         //set RS to send a data
    return;

}


/***************************** Helper definitions ****************************/

void lcd_delay()
{
    for(INT8U i = 0; i < DELAY_OUTER_LOOP; i++)
    {
        for(INT8U j = 0; j < DELAY_INNER_LOOP; j++)
        {
            //Wait for the LCD to understand the data
        }
    }
    return;
}

void flash_pin_E()
{
    GPIO_PORTD_DATA_R |= PIN_E;          //Flash the E pin (Port D, 4)
    GPIO_PORTD_DATA_R &= ~(PIN_E);
    return;
}

void clear_lcd_data_pins()
{
    GPIO_PORTC_DATA_R &= ~(0xF0);
    return;
}

void lcd_port_setup()
{
    //Enable clock for port C
    SYSCTL_RCGC2_R |= 0x04;

    //Set the direction (0=input and 1=output)
    GPIO_PORTC_DIR_R |= 0xF0;        //LCD D4-D7 -> PORTC 4-7

    //Enable the GPIO pins
    GPIO_PORTC_DEN_R |= 0xF0;

    //Enable internal pull-up for the high nibble
    GPIO_PORTC_PUR_R |= 0xF0;


    //Enable clock for port D
    SYSCTL_RCGC2_R |= 0x08;

    //Set the direction (0=input and 1=output)
    GPIO_PORTD_DIR_R |= 0x0C;        //LCD E -> PORTD 3

    //Enable the GPIO pins
    GPIO_PORTD_DEN_R |= 0x0C;        //LCD E -> PORTD 3

    //Enable internal pull-up for the high nibble
    GPIO_PORTD_PUR_R |= 0x0C;

    return;
}
