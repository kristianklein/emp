#ifndef FILE_FUNC_H
#define FILE_FUNC_H

#include <stdint.h>
#include "emp_type.h"

BOOLEAN uart0_put(INT8U character);
BOOLEAN uart0_get(INT8U* character);
BOOLEAN lcd_put(INT8U character);
BOOLEAN keypad_get(INT8U* character);
BOOLEAN button_get(INT8U* character);

#endif
