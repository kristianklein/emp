#include "file.h"
#include "global_def.h"
#include "file_func.h"

typedef struct
{
	BOOLEAN (*put)(INT8U);
	BOOLEAN (*get)(INT8U*);
} FCB_T;

FCB_T files[MAX_FILES];

void file_init(void)
{
	for (INT8U i = 0; i < MAX_FILES; i++)
	{
		files[i].put = NULL;
		files[i].get = NULL;
	}
	
	// Initialize files
	// Functions get/put character to/from respective queues
	files[UART0].put = uart0_put;
	files[UART0].get = uart0_get;
	files[LCD].put = lcd_put;
	files[KEYPAD].get = keypad_get;
	files[BUTTON].get = button_get;
	
	return;
}

BOOLEAN file_put(FILE file, INT8U character)
{
	BOOLEAN result = 0;
	
	if (files[(INT8U)file].put) // If file has a put-function
	{
		result = files[(INT8U)file].put(character); // Put character in file
	}
	
	return result;
}

BOOLEAN file_get(FILE file, INT8U* character)
{
	BOOLEAN result = 0;
	
	if (files[(INT8U)file].get) // If file has a get-function
	{
		result = files[(INT8U)file].get(character); // Get a character and place it where the pointer is pointing
	}
	
	return result;
}
