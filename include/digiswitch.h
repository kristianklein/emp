/*
 * digiswitch.h
 *
 *  Created on: 25. maj 2018
 *      Author: Anders Sørensen
 */

#ifndef INCLUDE_DIGISWITCH_H_
#define INCLUDE_DIGISWITCH_H_


void digi_init();
/*****************************************************************************
* Input : -
* Output : -
* Function : This function initialize the digi-switch
******************************************************************************/

INT8U digi_turn();
/*****************************************************************************
* Input : -
* Output : A CW turn (1), a CCW turn (2), or no event (0)
* Function : This function reads the digi-switch and returns the rotation or press event
******************************************************************************/

INT8U digi_press();
/*****************************************************************************
* Input : -
* Output : 1 if pressed
* Function : This function registers if the digi switch is pressed
*****************************************************************************/


#endif /* INCLUDE_DIGISWITCH_H_ */
