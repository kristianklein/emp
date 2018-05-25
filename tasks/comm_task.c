#include "comm_task.h"
#include "queue.h"
#include "log.h"
#include "string.h"

extern xQueueHandle xQueueUART_RX;
extern xQueueHandle xQueueUART_TX;

typedef enum {IDLE, SET_PRICE, GET_LOGS, GET_CASH_SUM, GET_ACC_SUM, GET_OP_TIME} COMM_STATE_T;

static void comm_print_sorted_logs(void);
static void comm_print_log(LOG_T log);

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

                // TODO: Make some way to set the product price. Should be in the petrol stand module?

                /*
                // Put test logs
                log_put(8, 30, 21, GAS_92, 22500, 150, 1, 0); // Cash
                log_put(8, 30, 21, GAS_95, 21100, 350, 0, 449120); // Account
                log_put(8, 30, 21, GAS_92, 42300, 250, 0, 229312); // Account
                log_put(8, 30, 21, GAS_92, 23500, 100, 1, 0); // Cash
                log_put(8, 30, 21, GAS_E10, 500, 350, 0, 449392); // Account
                log_put(8, 30, 21, GAS_95, 11500, 500, 0, 449120); // Account
                */

                state = IDLE;
                break;

            case GET_LOGS:
                comm_print_sorted_logs();
                state = IDLE;
                break;

            case GET_CASH_SUM:;
                INT32U cash_sum = log_get_cash_sum();
                gfprintf(UART0, "Cash purchases: %d\r\n", cash_sum);
                state = IDLE;
                break;

            case GET_ACC_SUM:;
                INT32U acc_sum = log_get_acc_sum();
                gfprintf(UART0, "Account purchases: %d\r\n", acc_sum);
                state = IDLE;
                break;

            case GET_OP_TIME:
                gprintf("Entered GET_OP_TIME state. Going back to IDLE. \r\n");

                // TODO: Implement an "operating time" variable in LOG_T

                state = IDLE;
                break;

            default:
                break;
        }
    }
}

void comm_print_sorted_logs()
{
    INT8U num_logs = log_capacity();
    LOG_T current_log;

    gfprintf(UART0, "Total number of purchases: %d\r\n", num_logs);
    gfprintf(UART0, "Time - Product - Quantity - Price - Payment method - Account\r\n");

    // Print GAS_92
    for (INT8U i = 0; i < num_logs; i++)
    {
        current_log = log_get(i);
        if (current_log.product == GAS_92)
        {
            comm_print_log(current_log);
        }
    }

    // Print GAS_95
    for (INT8U i = 0; i < num_logs; i++)
    {
        current_log = log_get(i);
        if (current_log.product == GAS_95)
        {
            comm_print_log(current_log);
        }
    }

    // Print GAS_E10
    for (INT8U i = 0; i < num_logs; i++)
    {
        current_log = log_get(i);
        if (current_log.product == GAS_E10)
        {
            comm_print_log(current_log);
        }
    }

    return;
}

void comm_print_log(LOG_T log)
{
    char* payment_str = "ACCOUNT";
    char* gas_str = "E10";

    if (log.paid_cash)
    {
        payment_str = "CASH";
    }

    if (log.product == GAS_92)
    {
        gas_str = "92";
    }
    else if (log.product == GAS_95)
    {
        gas_str = "95";
    }

    gfprintf(UART0, "%02d:%02d:%02d - %s - %d - %d - %s - %d\r\n", log.hour, log.min, log.sec, gas_str, log.quantity, log.price, payment_str, log.account_num);
}
