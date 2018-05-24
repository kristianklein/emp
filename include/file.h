#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#include "emp_type.h"

typedef INT8U* EMP_FILE;

#define UART0 		0
#define LCD 		1
#define KEYPAD 		2
#define BUTTON		3

#define MAX_FILES 8

void file_init(void);
BOOLEAN file_put(EMP_FILE file, INT8U character);
BOOLEAN file_get(EMP_FILE file, INT8U* character);
BOOLEAN file_peek(EMP_FILE file, INT8U* character);

#endif
