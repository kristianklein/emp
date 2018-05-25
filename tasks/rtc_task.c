#include "rtc_task.h"
#include "rtc.h"
#include "emp_type.h"
#include "rgb.h"


// DEBUGGING
#include "queue.h"
#include "string.h"

extern xQueueHandle xQueueMillilitersFueled;
// DEBUGGING

#define TICKS_PER_SEC 200

void rtc_task(void *pvParameters)
{
    BOOLEAN led_on = 0;

    while(1)
    {
        // Increment RTC seconds once every 200 ticks (every second)
        rtc_increment_sec();

        // Toggle LED
        //led_on ^= 1;
        //rgb_set(led_on, 0, 0);


        // DEBUGGING
        INT32U fuel;
        if (xQueueReceive(xQueueMillilitersFueled, &fuel, 0))
        {
            gfprintf(UART0, "Fueled: %06d\r\n", fuel);
        }
        // DEBUGGING

        vTaskDelay((portTickType)TICKS_PER_SEC);
    }
}
