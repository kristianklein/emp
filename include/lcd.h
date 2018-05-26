#ifndef LCD_DISP_H
#define LCD_DISP_H

// RS = Register select (0 = instruction register, 1 = data register)
// E = Enable (1 = starts data read/write)
// RW = Tied to ground (always write)
// DB4-7 = data bus pins (Send high bits first, then low bits!)
// DB7 can be used as Busy Flag, but only when RW is 1 (thus not possible here)

#include <stdint.h>
#include "emp_type.h"

#define LCD_CLEAR 0x1B // ESC character

void lcd_init(void);
void lcd_write_4bit(INT8U value, BOOLEAN is_instruction);
void lcd_write_8bit(INT8U value, BOOLEAN is_instruction);
void lcd_clear(void);
void lcd_new_line(void);
void lcd_us_delay(INT16U usec);
void lcd_cursor_home(void);
void lcd_cursor_left(INT8U reps);
void lcd_cursor_right(INT8U reps);
void lcd_print_char(INT8U character);
void lcd_print_str(INT8U string[]);
INT8U lcd_int2char(INT8U value);

#endif
