#ifndef TASKS_UART_TASK_H_
#define TASKS_UART_TASK_H_

#include "FreeRTOS.h"
#include "task.h"

void uart_rx_task(void *pvParameters);
void uart_tx_task(void *pvParameters);

#endif
