#include "uart_task.h"
#include "emp_type.h"
#include "queue.h"
#include "uart0.h"

extern xQueueHandle xQueueUART_RX;
extern xQueueHandle xQueueUART_TX;

void uart_rx_task(void *pvParameters)
{
    INT8U received_value;

    while(1)
    {
        if (uart0_rx_rdy())
        {
            // Get data and put it in the RX queue
            received_value = uart0_getc();
            xQueueSendToBack(xQueueUART_RX, (void *) &received_value, 10);
        }
    }
}

void uart_tx_task(void *pvParameters)
{
    INT8U transmit_value;

    while(1)
    {
        if (xQueueReceive(xQueueUART_TX, &transmit_value, 10))
        {
            // Wait for TX to be ready, then send value
            while (!uart0_tx_rdy());

            uart0_putc(transmit_value);
        }
    }
}
