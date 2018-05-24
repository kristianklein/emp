#include "rtc_task.h"
#include "rtc.h"
#include "emp_type.h"
#include "rgb.h"

#define TICKS_PER_SEC 200


// DEBUGGING
#include "string.h"

// DEBUGGING


void rtc_task(void *pvParameters)
{
    BOOLEAN led_on = 0;

    while(1)
    {
        // Increment RTC seconds once every 200 ticks (every second)
        vTaskDelay(TICKS_PER_SEC);
        rtc_increment_sec();

        // Toggle LED
        led_on ^= 1;
        rgb_set(led_on, 0, 0);

        // DEBUGGING
        gfprintf(UART0, "Hello\r\n");
        // DEBUGGING
    }
    return;
}
