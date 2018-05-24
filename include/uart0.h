#ifndef UART0_H
#define UART0_H

#include <stdint.h>
#include "emp_type.h"

BOOLEAN uart0_rx_rdy();
INT8U uart0_getc();
BOOLEAN uart0_tx_rdy();
void uart0_putc(INT8U ch);
void uart0_init(INT32U baud_rate, INT8U databits, INT8U stopbits, INT8U parity);

#endif

