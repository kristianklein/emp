#include "comm_task.h"
#include "queue.h"
#include "log.h"
#include "string.h"

extern xQueueHandle xQueueUART_RX;
extern xQueueHandle xQueueUART_TX;

typedef enum {IDLE, SET_PRICE, GET_LOGS, GET_CASH_SUM, GET_ACC_SUM, GET_OP_TIME} COMM_STATE_T;

void comm_task(void *pvParameters)
{
    static COMM_STATE_T state = IDLE;
    INT8U received_value;

    while (1)
    {
        switch (state)
        {
            case IDLE:
                // Wait for command from PC
                if (xQueueReceive(xQueueUART_RX, &received_value, 10))
                {
                    switch (received_value)
                    {
                        case 'p':
                            state = SET_PRICE;
                            break;
                        case 'l':
                            state = GET_LOGS;
                            break;
                        case 'c':
                            state = GET_CASH_SUM;
                            break;
                        case 'a':
                            state = GET_ACC_SUM;
                            break;
                        case 't':
                            state = GET_OP_TIME;
                            break;
                    }
                }
                break;

            case SET_PRICE:
                gprintf("Entered SET_PRICE state. Going back to IDLE. \r\n");
                state = IDLE;
                break;

            case GET_LOGS:
                gprintf("Entered GET_LOGS state. Going back to IDLE. \r\n");
                state = IDLE;
                break;

            case GET_CASH_SUM:;
                gprintf("Entered GET_CASH_SUM state. Going back to IDLE. \r\n");
                state = IDLE;
                break;

            case GET_ACC_SUM:;
                gprintf("Entered GET_ACC_SUM state. Going back to IDLE. \r\n");
                state = IDLE;
                break;

            case GET_OP_TIME:
                gprintf("Entered GET_OP_TIME state. Going back to IDLE. \r\n");
                state = IDLE;
                break;

            default:
                break;
        }
    }
}
