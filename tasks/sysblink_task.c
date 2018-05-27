#include "sysblink_task.h"
#include "sysblink.h"
#include "rgb.h"

#define TICKS_PER_SEC 200

void sysblink_task(void *pvParameters)
{
    sysblink_init();
    while (1)
    {
        sysblink_toggle_led();
        vTaskDelay((portTickType)TICKS_PER_SEC);
    }
}
