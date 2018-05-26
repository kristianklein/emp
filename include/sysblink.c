#include "sysblink.h"
#include "tm4c123gh6pm.h"

#define STATUS_LED 0b01000000

void sysblink_init(void)
{
	// Enable clock for PORTD
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;
	
	// Set status LED as output
	GPIO_PORTD_DIR_R |= STATUS_LED;
	GPIO_PORTD_DEN_R |= STATUS_LED;
	GPIO_PORTD_PUR_R &= ~(STATUS_LED);
	
	return;
}

void sysblink_toggle_led(void)
{
	GPIO_PORTD_DATA_R ^= 0b01000000;
	
	return;
}
