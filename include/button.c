#include "button.h"
#include "tm4c123gh6pm.h"

#define SW1 1
#define SW2 2

void button_init(void)
{
    // Setup buttons on Tiva
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // Enable clock for PORTF
    GPIO_PORTF_LOCK_R = 0x4C4F434B; // Unlock the CR register
    GPIO_PORTF_CR_R = 0xFF; // Enable overwrite of PUR to FP0
    GPIO_PORTF_DIR_R &= ~(0b00010001); // Set SW1 and SW2 as input
    GPIO_PORTF_DEN_R |= 0b00010001; // Digital enable for SW1, SW2
    GPIO_PORTF_PUR_R |= 0b00010001; // Enable pull-up resistor for SW1 and SW2

    return;
}

BOOLEAN button_active(INT8U button_number)
{
	BOOLEAN active = 0;
	
	switch (button_number)
	{
		case SW1:
			if ( !(GPIO_PORTF_DATA_R & 0b00010000) )
			{
				active = 1;
			}
			break;
		case SW2:
			if ( !(GPIO_PORTF_DATA_R & 0b00000001) )
			{
				active = 1;
			}
			break;
		default:
			break;
	}

	return active;
}
