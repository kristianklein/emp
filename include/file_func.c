#include "file_func.h"
#include "FreeRTOS.h"
#include "queue.h"

extern xQueueHandle xQueueUART_RX;
extern xQueueHandle xQueueUART_TX;
extern xQueueHandle xQueueButton;
extern xQueueHandle xQueueKeypad;
extern xQueueHandle xQueueLCD;

BOOLEAN uart0_put(INT8U character)
{
	BOOLEAN result = 0;
	
	if (xQueueSendToBack(xQueueUART_TX, (void *) &character, 10))
	{
		result = 1;
	}		
	
	return result;
}


BOOLEAN uart0_get(INT8U* character)
{
	BOOLEAN result = 0;
	
	INT8U returned_value;

	if (xQueueReceive(xQueueUART_RX, &returned_value, 10))
	{
	    *character = returned_value;

	    result = 1;
	}
	
	return result;
}

BOOLEAN lcd_put(INT8U character)
{
	BOOLEAN result = 0;
	
	if (xQueueSendToBack(xQueueLCD, (void *) &character, 10))
	{
	    result = 1;
	}		
	
	return result;
}

BOOLEAN keypad_get(INT8U* character)
{
	BOOLEAN result = 0;
	
	INT8U returned_value;

	if (xQueueReceive(xQueueKeypad, &returned_value, 10))
	{
		*character = returned_value;
		
		result = 1;
	}
	
	return result;
}

BOOLEAN button_get(INT8U* character)
{
	BOOLEAN result = 0;
	
	INT8U returned_value;

	if (xQueueReceive(xQueueButton, &returned_value, 10))
	{
		*character = returned_value;

		result = 1;
	}
	
	return result;
}
