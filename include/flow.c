#include "flow.h"
#include "tm4c123gh6pm.h"
#include "rgb.h"

BOOLEAN running = 0;
BOOLEAN shunt = 0;
INT32U pulses = 0;

void flow_init()
{
    __asm("cpsid i"); // Disable global interrupts
    SYSCTL_RCGCTIMER_R = SYSCTL_RCGCTIMER_R | SYSCTL_RCGCTIMER_R0; // Enable clock for TIMER0
    TIMER0_CTL_R = 0x00; // Disable timer before configuring
    TIMER0_CFG_R = TIMER_CFG_16_BIT; // 16-bit mode
    TIMER0_TAMR_R = (TIMER_TAMR_TAMR_PERIOD); // Timer mode: Periodic, down-counting
    TIMER0_TAILR_R = 35556; // Interval load register (timer start value)
    TIMER0_IMR_R = TIMER_IMR_TATOIM; // Interrupt mask register: Timeout interrupt
    TIMER0_TAPS_R = 2; // Prescaler
    NVIC_EN0_R = (1<<19); // Enable TIMER0A interrupt in NVIC
    TIMER0_CTL_R = TIMER_CTL_TAEN; // Enable TIMER0A
    __asm("cpsie i"); // Enable global interrupts

    return;
}

void flow_reset()
{
    // Reset pump variables
    running = 0;
    shunt = 0;
    pulses = 0;

    return;
}

INT32U flow_get_pulses()
{
    return pulses;
}

void flow_run_mode(BOOLEAN mode)
{
    running = mode;

    return;
}

void flow_shunt_mode(BOOLEAN mode)
{
    shunt = mode;

    return;
}

void timer0a_isr()
{
    static pulse_prescale = 10;

    // Clear interrupt flag and reset timer value
    TIMER0_ICR_R = 0xFF;//0b00000001;

    // Do your thing

    // Check if pump is running
    if (running)
    {
        if (shunt)
        {
            if (!--pulse_prescale) // If shunt is on, use prescaler to only give a pulse every tenth interrupt
            {
                pulses++;
                pulse_prescale = 10;
            }
        }
        else // If shunt is off, give 1 pulse per interrupt
        {
            pulses++;
        }
    }

    return;
}
