#include "pump_task.h"
#include "queue.h"
#include "button.h"
#include "rgb.h"
#include "flow.h"

typedef enum {IDLE, READY, SHUNT, RUNNING} PUMP_STATE_T;

extern xQueueHandle xQueueButton;

void pump_task(void *pvParameters)
{
    PUMP_STATE_T state = IDLE;
    INT8U received_value;
    INT32U last_pulse;

    while (1)
    {
        switch (state)
        {
            case IDLE:
                if (xQueueReceive(xQueueButton, &received_value, 10))
                {
                    if (received_value == SW2_CLICKED) // Trunk taken off handle
                    {
                        flow_reset();
                        state = READY;
                    }
                }
                break;
            case READY:
                if (xQueueReceive(xQueueButton, &received_value, 10))
                {
                    if (received_value == SW2_CLICKED) // Trunk put back on handle
                    {
                        flow_reset(); // Reset everything before going back to IDLE
                        flow_run_mode(0);
                        flow_shunt_mode(0);
                        state = IDLE;
                    }
                    if (received_value == SW1_CLICKED) // Started fueling
                    {
                        rgb_set(1,0,0); // Turn on red LED while pumping and yellow (blue) while shunt is active
                        flow_shunt_mode(1);
                        flow_run_mode(1);
                        last_pulse = flow_get_pulses(); // Last registrered pulse reading (to use shunt properly)
                        state = SHUNT;
                    }
                }
                break;
            case SHUNT:
                if (xQueueReceive(xQueueButton, &received_value, 10))
                {
                    if (received_value == SW1_RELEASED) // Stopped fueling
                    {
                        rgb_set(1,1,1); // Turn off LEDs
                        state = READY;
                    }
                }

                if (flow_get_pulses()-last_pulse > PULSES_PER_100ML) // Turn off shunt
                {
                    rgb_set(1,0,1);
                    flow_shunt_mode(0);
                    state = RUNNING;
                }
                break;
            case RUNNING:
                if (xQueueReceive(xQueueButton, &received_value, 10))
                {
                    if (received_value == SW1_RELEASED) // Stopped fueling
                    {
                        flow_run_mode(0);
                        rgb_set(1,1,1); // Turn off LEDs
                        state = READY;
                    }
                }

                break;
            default:
                break;
        }
    }
}
