#include "ui_task.h"

typedef enum {WELCOME, PAYMENT, CASH, ACCOUNT, PIN, PUMP_READY, FUELING_DONE} UI_STATE_T;

void ui_task(void *pvParameters)
{
    UI_STATE_T state = WELCOME;

    while (1)
    {
        switch (state)
        {

            // Det er tilladt at lave hjælpe funktioner internt i denne fil,
            // for at gøre dette switch-statement lidt mere overskueligt :)

            case WELCOME:
                break;
            case PAYMENT:
                break;
            case CASH:
                break;
            case ACCOUNT:
                break;
            case PIN:
                break;
            case PUMP_READY:
                break;
            case FUELING_DONE:
                break;
            default:
                break;
        }
    }
}
