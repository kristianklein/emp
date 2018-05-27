/*
 * lcd.h
 *
 *  Created on: 24. maj 2018
 *      Author: Anders Sørensen
 */

#ifndef LCD_H_
#define LCD_H_


#include <stdint.h>
#include "emp_type.h"



void lcd_init();
/*****************************************************************************
* Input : -
* Output : -
* Function : This function initialize the LCD-display
******************************************************************************/

void lcd_write(INT8U char_value);
/*****************************************************************************
* Input : The char value to be written
* Output : -
* Function : This function writes a char value on the display
******************************************************************************/

void lcd_newline();
/*****************************************************************************
* Input : -
* Output : -
* Function : This function changes the line on the display
******************************************************************************/

void lcd_clear();
/*****************************************************************************
* Input : -
* Output : -
* Function : This function clears the display
******************************************************************************/

void lcd_return_home();
/*****************************************************************************
* Input : -
* Output : -
* Function : This function sets the cursor to the first point.
******************************************************************************/







#endif /* LCD_H_ */
