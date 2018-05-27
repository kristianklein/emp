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
#define WAIT_FOR_LCD_READY          1       //Important! if the time slice is size changed, then change this number

//PIN defines
#define PIN_E                       0x08


/***************************** Constants *************************************/
/***************************** Variables *************************************/

/***************************** Helper Functions decelerations ****************/
void lcd_delay(INT32U delaytime);
void flash_pin_E();
void clear_lcd_data_pins();
void lcd_port_setup();
void lcd_write_4bit(INT8U value);
/***************************** Functions *************************************/

void lcd_init()
{
    // PC4-7: D4-7
    // PD2: RS
    //High nibble first


    lcd_port_setup();                   //Initialize the ports
    vTaskDelay(5);                      //Long startup delay
    GPIO_PORTD_DATA_R &= ~(0x04);       //Chance the LCD-to command mode
    lcd_delay(1000);                    //Small delay

    //Set Interface data to 4 bit - start
    lcd_write(0x30);                    //Send to 4 bit mode
    vTaskDelay(1);                      //Wait min 5ms (change this one day)
    lcd_write(0x30);                    //Set to 4 bit mode again.
    lcd_delay(50000);                   //Wait 100uS minimum
    lcd_write(0x32);                    // Send 4 bit index again to be sure
    //Set Interface data to 4 bit - end

    lcd_write(0x28);                    //Set Interface data to 4 bit, number of lines to 2 lines and fond to 5x8
    lcd_write(0x01);                    //Clear the display
    lcd_write(0x06);                    //Set the direction
    lcd_write(0x0C);                    //Turn on the display
    lcd_write(0x02);                    //Set the cursor home

    //Make ready to write
    GPIO_PORTD_DATA_R |= 0x04;          //Set RS to write
    lcd_delay(1000);                    //Small delay
    return;
}


void lcd_write(INT8U char_value)
{
    INT8U temp = char_value & 0xF0;

    clear_lcd_data_pins();
    GPIO_PORTC_DATA_R |= temp;
    flash_pin_E();
    lcd_delay(500);                    //Delay the 150us to give the display time to save the data

     temp = char_value << 4;            //Move low nibble to high

     clear_lcd_data_pins();
     GPIO_PORTC_DATA_R |= temp;         //Set the 4-bit command
     flash_pin_E();
     lcd_delay(2400);                   //Delay the 150us to give the display time to save the data

    return;
}


void lcd_newline()
{
    clear_lcd_data_pins();
    GPIO_PORTD_DATA_R &= ~(0x04);       //Chance the LCD-to command mode
    lcd_delay(500);                    //Small delay
    lcd_write(0xC0);
    GPIO_PORTD_DATA_R |= 0x04;          //Set RS to write
    lcd_delay(500);                    //Small delay
    return;
}

void lcd_clear()
{
    GPIO_PORTD_DATA_R &= ~(0x04);      //Clear RS to make a command
    lcd_delay(500);                   //Small delay
    lcd_write(0x01);                   //Clears the display
    GPIO_PORTD_DATA_R |= 0x04;         //set RS to send a data
    lcd_delay(500);                   //Small delay
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

void lcd_delay(INT32U delaytime)
{
    for(INT32U i = 0; i < delaytime; i++)
    {

    }
    return;
}

void flash_pin_E()
{
    GPIO_PORTD_DATA_R |= PIN_E;          //Flash the E pin (Port D, 4)
//    lcd_delay(1000);
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
    SYSCTL_RCGCGPIO_R |= (SYSCTL_RCGCGPIO_R2 | SYSCTL_RCGCGPIO_R3);   //Enable clock for port C and D

    //Enable clock for port C and D
//    SYSCTL_RCGC2_R |= (0x04 | 0x08);  //Enable clock for port C and D


    GPIO_PORTC_DIR_R |= 0xF0;           //Set the direction (0=input and 1=output)
    GPIO_PORTC_DEN_R |= 0xF0;           //Enable the GPIO pins
    GPIO_PORTC_PUR_R |= 0xF0;           //Enable internal pull-up for the high nibble
    GPIO_PORTD_DIR_R |= 0x0C;           //Set the direction (0=input and 1=output)
    GPIO_PORTD_DEN_R |= 0x0C;           //Enable the GPIO pins
    GPIO_PORTD_PUR_R |= 0x0C;           //Enable internal pull-up for the high nibble

    return;
}

void lcd_write_4bit(INT8U value)
{
    value = ~(value & 0x0F) << 4;
    clear_lcd_data_pins();
    GPIO_PORTC_DATA_R |= value;
    flash_pin_E();
    return;
}
