#include "rtc_task.h"
#include "rtc.h"
#include "emp_type.h"
#include "rgb.h"


#define TICKS_PER_SEC 200

void rtc_task(void *pvParameters)
{
    while(1)
    {
        // Increment RTC seconds once every 200 ticks (every second)
        rtc_increment_sec();

        vTaskDelay((portTickType)TICKS_PER_SEC);
    }
}
