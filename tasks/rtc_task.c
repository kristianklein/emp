#include "rtc_task.h"
#include "rtc.h"
#include "emp_type.h"
#include "rgb.h"

#define TICKS_PER_SEC 200

void rtc_task(void *pvParameters)
{
    BOOLEAN led_on = 0;

    while(1)
    {
        // Increment RTC seconds once every 200 ticks (every second)
        rtc_increment_sec();

        // Toggle LED
        led_on ^= 1;
        rgb_set(led_on, 0, 0);

        vTaskDelay((portTickType)TICKS_PER_SEC);
    }
}
