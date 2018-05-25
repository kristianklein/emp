#include "pump_task.h"
#include <stdint.h>
#include "emp_type.h"
#include "queue.h"
#include "button.h"
#include "rgb.h"
#include "flow.h"
#include "semphr.h"

typedef enum {IDLE, READY, SHUNT, RUNNING} PUMP_STATE_T;

extern xQueueHandle xQueueButton;
extern xQueueHandle xQueueMaxMilliliters;
extern xQueueHandle xQueueMillilitersFueled;
extern xSemaphoreHandle xSemaphorePumpActive;

void pump_task(void *pvParameters)
{
    PUMP_STATE_T state = IDLE;
    INT8U received_value;
    INT32U last_pulse;
    INT32U max_milliliters;
    INT32U max_pulses = 1125; // This will be set to 0 if customer pays from account

    while (1)
    {
        if (xQueueReceive(xQueueMaxMilliliters, &max_milliliters, 10)) // Check buffer for new max
        {
            // Calculate number of pulses (1 L = 1125 pulses, 100 mL = )
            max_pulses = ((FP32)max_milliliters / 1000) * 1125; // Cast to FP32 for accurate calculation
        }

        switch (state)
        {
            case IDLE:
                if (xQueueReceive(xQueueButton, &received_value, 10))
                {
                    if (received_value == SW2_CLICKED) // Trunk taken off handle
                    {
                        if (xSemaphoreTake(xSemaphorePumpActive, 1)) // Start pump if semaphore is available
                        {
                            flow_reset();
                            state = READY;
                        }

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

                        // Put number of milliliters tanked in queue
                        INT32U milliliters_fueled = (flow_get_pulses() / PULSES_PER_100ML) * 100;
                        xQueueSendToBack(xQueueMillilitersFueled, &milliliters_fueled, 10);

                        state = IDLE;
                    }
                    if (received_value == SW1_CLICKED) // Started fueling
                    {
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
                        flow_shunt_mode(0);
                        flow_run_mode(0);
                        state = READY;
                    }
                }

                // Soft stop
                if (flow_get_pulses() >= max_pulses - PULSES_PER_100ML)
                {
                    if (flow_get_pulses() >= max_pulses) // Limit reached! End fueling.
                    {
                        flow_run_mode(0);
                        flow_shunt_mode(0);
                        state = READY;
                    }

                    break; // Dirty code! Necessary to avoid running the "Soft start" code below
                }

                // Soft start
                if (flow_get_pulses()-last_pulse >= PULSES_PER_100ML) // Turn off shunt
                {
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
                        state = READY;
                    }
                }

                if (max_pulses && max_pulses - flow_get_pulses() < PULSES_PER_100ML)
                {
                    flow_shunt_mode(1);
                    last_pulse = flow_get_pulses();
                    state = SHUNT;
                }


                break;
            default:
                break;
        }
    }
}
