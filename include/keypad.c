#include "keypad.h"
#include "tm4c123gh6pm.h"


#define KEYB_D 0b00000100
#define KEYB_E 0b00001000
#define KEYB_F 0b00010000
#define KEYB_G 0b00000001
#define KEYB_H 0b00000010
#define KEYB_J 0b00000100
#define KEYB_K 0b00001000

#define NUM_COLS 3
#define NUM_ROWS 4

/* 
 * KEYB_D (PA2) = X3
 * KEYB_E (PA3) = X2
 * KEYB_F (PA4) = X1
 * KEYB_G (PE0) = Y4
 * KEYB_H (PE1) = Y3
 * KEYB_J (PE2) = Y2
 * KEYB_K (PE3) = Y1
 */
 
void keypad_init(void)
{
	// Enable clocks for PORTA and PORTE
	SYSCTL_RCGCGPIO_R |= (SYSCTL_RCGCGPIO_R0 | SYSCTL_RCGCGPIO_R4);
	
	// Set columns as output
	GPIO_PORTA_DIR_R |= (KEYB_D | KEYB_E | KEYB_F);
	GPIO_PORTA_DEN_R |= (KEYB_D | KEYB_E | KEYB_F);
	GPIO_PORTA_PUR_R &= ~(KEYB_D | KEYB_E | KEYB_F);
	GPIO_PORTA_DATA_R &= ~(KEYB_D | KEYB_E | KEYB_F);
	
	// Set rows as input without pullup (pull-down is already on the board)
	GPIO_PORTE_DIR_R &= ~(KEYB_G | KEYB_H | KEYB_J | KEYB_K);
	GPIO_PORTE_DEN_R |= (KEYB_G | KEYB_H | KEYB_J | KEYB_K);
	GPIO_PORTE_PUR_R &= ~(KEYB_G | KEYB_H | KEYB_J | KEYB_K);
	
	return;
}

void keypad_enable_column(INT8U column)
{
	switch (column)
	{
		case 1:
			GPIO_PORTA_DATA_R &= ~(KEYB_D | KEYB_E);
			GPIO_PORTA_DATA_R |= (KEYB_F);
			break;
		case 2:
			GPIO_PORTA_DATA_R &= ~(KEYB_D | KEYB_F);
			GPIO_PORTA_DATA_R |= (KEYB_E);
			break;
		case 3:
			GPIO_PORTA_DATA_R &= ~(KEYB_E | KEYB_F);
			GPIO_PORTA_DATA_R |= (KEYB_D);
			break;
		default:
			break;
	}
	
	return;
}

BOOLEAN keypad_key_active(INT8U row, INT8U column)
{
	BOOLEAN key_active = 0;
	
	keypad_enable_column(column);
	
	switch (row)
	{
		case 1:
			if (GPIO_PORTE_DATA_R & KEYB_K)
			{
				key_active = 1;
			}
			break;
		case 2:
			if (GPIO_PORTE_DATA_R & KEYB_J)
			{
				key_active = 1;
			}
			break;
		case 3:
			if (GPIO_PORTE_DATA_R & KEYB_H)
			{
				key_active = 1;
			}
			break;
		case 4:
			if (GPIO_PORTE_DATA_R & KEYB_G)
			{
				key_active = 1;
			}
			break;
		default:
			break;
	}
	
	return key_active;
}

INT8U keypad_key2char(INT8U row, INT8U column)
{
	INT8U character = 0;
	
	switch (column)
	{
		case 1:
			switch (row)
			{
				case 1:
					character = '1';
					break;
				case 2:
					character = '4';
					break;
				case 3:
					character = '7';
					break;
				case 4:
					character = '*';
					break;
			}
			break;
		case 2:
			switch (row)
			{
				case 1:
					character = '2';
					break;
				case 2:
					character = '5';
					break;
				case 3:
					character = '8';
					break;
				case 4:
					character = '0';
					break;
			}
			break;
		case 3:
			switch (row)
			{
				case 1:
					character = '3';
					break;
				case 2:
					character = '6';
					break;
				case 3:
					character = '9';
					break;
				case 4:
					character = '#';
					break;
			}
			break;
	}
	
	return character;
}

INT8U keypad_read(void)
{
	// Keep track of eachs key's previous state
	static INT8U key_state[NUM_ROWS][NUM_COLS] = {{0, 0, 0}, {0, 0, 0},	{0, 0, 0}, {0, 0, 0}};
	INT8U character = 0;
	
	for (INT8U i = 0; i < NUM_ROWS; i++) // Iterate over columns
	{
		for (INT8U j = 0; j < NUM_COLS; j++) // Iterate over rows in current column
		{
			BOOLEAN key_active = keypad_key_active(i+1, j+1);
			
			if (key_active && !key_state[i][j]) // Key was pressed
			{
				key_state[i][j] = 1;
				character = keypad_key2char(i+1, j+1);
				break;
			}
			else if (!key_active && key_state[i][j]) // Key was released
			{
				key_state[i][j] = 0;
			}
			
		}
	}
	
	return character;
}
