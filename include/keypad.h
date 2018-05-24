#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include "emp_type.h"

/*
 * TODO: Move non-interface functions to source-file
 *
 */

void keypad_init(void);
void keypad_enable_column(INT8U column);
BOOLEAN keypad_key_active(INT8U row, INT8U column);
INT8U keypad_key2char(INT8U row, INT8U column);
INT8U keypad_read(void); // Returns a char for the corresponding button pressed

#endif
