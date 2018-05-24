#include "rtc.h"

INT8U rtc_hour = 0, rtc_min = 0, rtc_sec = 0;

void rtc_set_hour(INT8U hour)
{
	rtc_hour = hour;
	
	return;
}

void rtc_set_min(INT8U min)
{
	rtc_min = min;
}

void rtc_set_sec(INT8U sec)
{
	rtc_sec = sec;
}

INT8U rtc_get_hour(void)
{
	return rtc_hour;
}

INT8U rtc_get_min(void)
{
	return rtc_min;
}

INT8U rtc_get_sec(void)
{
	return rtc_sec;
}


void rtc_increment_sec(void)
{
    if (rtc_sec == 59)
    {
        rtc_sec = 0;

        if (rtc_min == 59)
        {
            rtc_min = 0;

            if (rtc_hour == 23)
            {
                rtc_hour = 0;
            }
            else
            {
                rtc_hour++;
            }
        }
        else
        {
            rtc_min++;
        }
    }
    else
    {
        rtc_sec++;
    }

    return;
}
