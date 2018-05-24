#ifndef RTC_H
#define RTC_H

#include <stdint.h>
#include "emp_type.h"

void rtc_set_hour(INT8U hour);
void rtc_set_min(INT8U min);
void rtc_set_sec(INT8U sec);

INT8U rtc_get_hour(void);
INT8U rtc_get_min(void);
INT8U rtc_get_sec(void);

void rtc_increment_sec(void);

#endif
