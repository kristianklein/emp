#include "lcd.h"
#include "tm4c123gh6pm.h"

#define CLOCK_FREQ_MHZ 16
#define LCD_RS 0b00000100
#define LCD_E 0b00001000
#define LCD_DB4 0b00010000
#define LCD_DB5 0b00100000
#define LCD_DB6 0b01000000
#define LCD_DB7 0b10000000

void lcd_us_delay(INT16U usec)
{
	for (INT32U i = 0; i < usec * CLOCK_FREQ_MHZ; i++);
	
	return;
}

void lcd_init(void)
{
  // Setup GPIO
  // PC4-7: D4-7
  // PD2: RS
  // PD3: E
  SYSCTL_RCGCGPIO_R |= (SYSCTL_RCGCGPIO_R2 | SYSCTL_RCGCGPIO_R3);
  
	// Data bus
  GPIO_PORTC_DIR_R |= (LCD_DB4 | LCD_DB5 | LCD_DB6 | LCD_DB7);
  GPIO_PORTC_DEN_R |= (LCD_DB4 | LCD_DB5 | LCD_DB6 | LCD_DB7);
  GPIO_PORTC_PUR_R &= ~(LCD_DB4 | LCD_DB5 | LCD_DB6 | LCD_DB7);
  
	// RS and E
  GPIO_PORTD_DIR_R |= (LCD_RS | LCD_E);
  GPIO_PORTD_DEN_R |= (LCD_RS | LCD_E);
  GPIO_PORTD_PUR_R &= ~(LCD_RS | LCD_E);
  
	// Initialize display
	lcd_us_delay(40000);
	
	lcd_write_4bit(0x03, 1);
	lcd_us_delay(10000);
	
	lcd_write_4bit(0x03, 1);
	lcd_us_delay(200);
	
	lcd_write_4bit(0x03, 1);
	lcd_us_delay(100);
  
	lcd_write_4bit(0x02, 1);
	lcd_us_delay(100);
	
	lcd_write_8bit(0x28, 1);
	lcd_us_delay(100);
	
	lcd_write_8bit(0x08, 1);
	lcd_us_delay(100);
	
	lcd_write_8bit(0x0C, 1);
	lcd_us_delay(100);
	
	lcd_write_8bit(0x06, 1);
	lcd_us_delay(100);
	
	lcd_write_8bit(0x01, 1);
	lcd_us_delay(100);
	
	return;
}

void lcd_write_4bit(INT8U value, BOOLEAN is_instruction)
{
	value = (value & 0x0F) << 4; // Clear upper nibble and shift lower nibble up
	
	// Data bus
	GPIO_PORTC_DATA_R &= ~(0xF0); // Clear data bus pins
	GPIO_PORTC_DATA_R |= value; // Set data bus pins to value
	
	if (is_instruction) // If value is an instruction/command, set RS low
	{
		GPIO_PORTD_DATA_R &= ~(LCD_RS);
	}
	else
	{
		GPIO_PORTD_DATA_R |= LCD_RS;
	}
	
	// Toggle E with appopriate delay to send data to LCD
	GPIO_PORTD_DATA_R |= LCD_E;
	lcd_us_delay(50);
	GPIO_PORTD_DATA_R &= ~(LCD_E);
	
	return;
}

void lcd_write_8bit(INT8U value, BOOLEAN is_instruction)
{
    INT8U upper_nibble = (value & 0xF0) >> 4;
	INT8U lower_nibble = (value & 0x0F);
	
	lcd_write_4bit(upper_nibble, is_instruction);
	lcd_write_4bit(lower_nibble, is_instruction);
}

void lcd_clear(void)
{
	lcd_write_8bit(0x01, 1);
	
	return;
}

void lcd_new_line(void)
{
    lcd_write_8bit(0xC0, 1);

    return;
}

void lcd_cursor_home(void)
{
	lcd_write_8bit(0x02, 1);
  return;
}

void lcd_cursor_left(INT8U reps)
{
	lcd_write_8bit(0x10, 1);
	
	return;
}

void lcd_cursor_right(INT8U reps)
{
	lcd_write_8bit(0x14, 1);
	
	return;
}

void lcd_print_char(INT8U character)
{
	// Write to CG (character generation) or DD (display data) RAM
	lcd_write_8bit(character, 0);
	
	return;
}

void lcd_print_str(INT8U *string)
{
	while (*string) // Always terminate strings with \0
	{
		lcd_print_char(*string);
		string++; // Increment pointer address to next character in string
	}
	
  return;
}

INT8U lcd_int2char(INT8U value)
{
    return value + 48;
}
