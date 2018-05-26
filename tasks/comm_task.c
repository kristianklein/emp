#include "comm_task.h"
#include "queue.h"
#include "log.h"
#include "string.h"
#include "price.h"

extern xQueueHandle xQueueUART_RX;
extern xQueueHandle xQueueUART_TX;

typedef enum {IDLE, SET_PRICE, GET_LOGS, GET_CASH_SUM, GET_ACC_SUM, GET_OP_TIME} COMM_STATE_T;

static void comm_print_sorted_logs(void);
static void comm_print_log(LOG_T log);

void comm_task(void *pvParameters)
{
    static COMM_STATE_T state = IDLE;
    static INT8U num_received = 0;
    static INT16U new_price = 0;
    static GAS_T fuel_type = GAS_92;
    INT8U received_value;


    while (1)
    {
        switch (state)
        {
            case IDLE:
                // Wait for command from PC
                if (xQueueReceive(xQueueUART_RX, &received_value, 0))
                {
                    switch (received_value)
                    {
                        case 'p':
                            num_received = 0;
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
                gfprintf(UART0, "CHANGE GAS PRICE\r\nSelect fuel type: ");


                // Expecting the following format: xPPPP
                // where x = fuel type (0, 1 or 2) and PPPP is the price in cents

                while (num_received < 5)
                {
                    if (xQueueReceive(xQueueUART_RX, &received_value, 0))
                    {
                        received_value -= 48; // Convert char to int

                        switch (num_received++) // Increment num_received after switch evaluation
                        {
                            case 0: // Fuel type
                                switch (received_value)
                                {
                                    case 0: // 92
                                        fuel_type = GAS_92;
                                        gfprintf(UART0, "\r\nEnter new price for 92 octane: ");
                                        break;
                                    case 1: // 95
                                        fuel_type = GAS_95;
                                        gfprintf(UART0, "\r\nEnter new price for 95 octane: ");
                                        break;
                                    case 2: // E10
                                        fuel_type = GAS_E10;
                                        gfprintf(UART0, "\r\nEnter new price for E10 bio-fuel: ");
                                        break;
                                    default: // default to 92
                                        fuel_type = GAS_92;
                                        gfprintf(UART0, "\r\nEnter new price for 92 octane: ");
                                        break;
                                }
                                break;
                            case 1: // Dollar tens
                                new_price = received_value * 1000;
                                gfprintf(UART0, "%01d", received_value);
                                break;
                            case 2: // Dollar ones
                                new_price += received_value * 100;
                                gfprintf(UART0, "%01d", received_value);
                                break;
                            case 3: // Cent tens
                                new_price += received_value * 10;
                                gfprintf(UART0, "%01d", received_value);
                                break;
                            case 4: // Cent ones
                                new_price += received_value;
                                gfprintf(UART0, "%01d\r\n", received_value);
                                break;
                            default:
                                break;
                        }
                    }
                }

                // All values received! Update price and return to IDLE state
                price_set(fuel_type, new_price);
                gfprintf(UART0, "Price set to %02d.%02d per liter!\r\n", new_price/100, new_price%100);

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

            case GET_OP_TIME:;
                //gprintf("Entered GET_OP_TIME state. Going back to IDLE. \r\n");
                INT32U op_time_sum = log_get_op_time();
                gfprintf(UART0, "Total operating time: %d\r\n", op_time_sum);
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
