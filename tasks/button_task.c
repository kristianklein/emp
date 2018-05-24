#include "button_task.h"
#include "button.h"
#include "queue.h"
#include "task.h"

typedef enum {
    IDLE,
    CLICKED,
    RELEASED
} BUTTON_STATE_T;

typedef enum {
    SW1_CLICKED,
    SW1_RELEASED,
    SW2_CLICKED,
    SW2_RELEASED
} BUTTON_EVENT_T;

extern xQueueHandle xQueueButton;

void button1_task(void *pvParameters){
    // Setup
    static BUTTON_STATE_T state = IDLE;

    button_init();

    // Super loop
    for (;;)
    {
        switch (state)
        {
            case IDLE:
                if (button_active(SW1)) // Button clicked!
                {
                    state = CLICKED; // Set state
                    BUTTON_EVENT_T event = SW1_CLICKED;
                    xQueueSendToBack(xQueueButton, (void *) &event, 10); // Send event to queue
                    vTaskDelay(1); // Delay 1 tick (5 ms)
                }
                break;
            case CLICKED:
                if (!button_active(SW1)) // Button released!
                {
                    state = RELEASED;
                    BUTTON_EVENT_T event = SW1_RELEASED;
                    xQueueSendToBack(xQueueButton, (void *) &event, 10);
                    vTaskDelay(1);
                }
                break;
            case RELEASED:
                // Return to idle state
                state = IDLE;
                break;
        }

    }

    return;
}

void button2_task(void *pvParameters){

    // Setup
    static BUTTON_STATE_T state = IDLE;

    button_init();

    // Super loop
    for (;;)
    {
        switch (state)
        {
            case IDLE:
                if (button_active(SW2)) // Button clicked!
                {
                    state = CLICKED; // Set state
                    BUTTON_EVENT_T event = SW2_CLICKED;
                    xQueueSendToBack(xQueueButton, (void *) &event, 10); // Send event to queue
                    vTaskDelay(1); // Delay 1 tick (5 ms)
                }
                break;
            case CLICKED:
                if (!button_active(SW2)) // Button released!
                {
                    state = RELEASED;
                    BUTTON_EVENT_T event = SW2_RELEASED;
                    xQueueSendToBack(xQueueButton, (void *) &event, 10);
                    vTaskDelay(1);
                }
                break;
            case RELEASED:
                // Return to idle state
                state = IDLE;
                break;
        }

    }

    return;
}


