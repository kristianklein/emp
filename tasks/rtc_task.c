#include "rtc_task.h"
#include "rtc.h"

#define TICKS_PER_SEC 200

void rtc_task(void *pvParameters)
{
    while(1)
    {
        // Increment RTC seconds once every 200 ticks (every second)
        vTaskDelay(TICKS_PER_SEC);
        rtc_increment_sec();
    }
    return;
}
