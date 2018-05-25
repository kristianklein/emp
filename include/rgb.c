#include "rgb.h"
#include "tm4c123gh6pm.h"

void rgb_init(void)
{
    // Setup RGB on Tiva
    SYSCTL_RCGC2_R |= 0x20; // Enable clock for PORTF
    GPIO_PORTF_DIR_R |= 0b00001110; // Set RGB as output
    GPIO_PORTF_DEN_R |= 0b00001110; // Digital enable for RGB
    GPIO_PORTF_PUR_R &= ~(0b00001110); // Disable pull-up resistor

}

void rgb_set(INT8U red, INT8U green, INT8U blue)
{
    if (red)
    {
        GPIO_PORTF_DATA_R |= 0b00000010;
    }
    else
    {
        GPIO_PORTF_DATA_R &= ~(0b00000010);
    }

    if (green)
    {
        GPIO_PORTF_DATA_R |= 0b00001000;
    }
    else
    {
        GPIO_PORTF_DATA_R &= ~(0b00001000);
    }

    if (blue)
    {
        GPIO_PORTF_DATA_R |= 0b00000100;
    }
    else
    {
        GPIO_PORTF_DATA_R &= ~(0b00000100);
    }

    return;
}
