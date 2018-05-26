#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include "emp_type.h"

#define SW1 1
#define SW2 2

typedef enum {
    SW1_CLICKED,
    SW1_RELEASED,
    SW2_CLICKED,
    SW2_RELEASED
} BUTTON_EVENT_T;

void button_init(void);
BOOLEAN button_active(INT8U button_number);

#endif
