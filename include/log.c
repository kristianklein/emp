#include "log.h"

INT8U capacity = 0;

LOG_T logs[MAX_LOGS];

BOOLEAN log_put(INT8U hour, INT8U min, INT8U sec, GAS_T product, INT32U quantity, INT32U price, BOOLEAN paid_cash, INT32U account_num, INT32U op_time)
{
    BOOLEAN result = 0;

    if (! (capacity == MAX_LOGS))
    {
        logs[capacity].hour = hour;
        logs[capacity].min = min;
        logs[capacity].sec = sec;
        logs[capacity].product = product;
        logs[capacity].quantity = quantity;
        logs[capacity].price = price;
        logs[capacity].paid_cash = paid_cash;
        logs[capacity].account_num = account_num;
        logs[capacity].op_time = op_time;

        capacity++;

        result = 1;
    }

    return result;
}
LOG_T log_get(INT8U log_number)
{
    LOG_T log;

    if (log_number < MAX_LOGS)
    {
        log = logs[log_number];
    }

    return log;
}

INT8U log_capacity()
{
    return capacity;
}

INT32U log_get_cash_sum()
{
    INT32U sum = 0;

    // Add up all purchases paid for with cash
    for (INT8U i = 0; i < capacity; i++)
    {
        if (logs[i].paid_cash)
        {
            sum += logs[i].price;
        }
    }

    return sum;
}
INT32U log_get_acc_sum()
{
    INT32U sum = 0;

    // Add up all purchases NOT paid for with cash
    for (INT8U i = 0; i < capacity; i++)
    {
        if (!logs[i].paid_cash)
        {
            sum += logs[i].price;
        }
    }

    return sum;
}


INT32U log_get_op_time()
{
    INT32U sum = 0;

    for (INT8U i = 0; i < capacity; i++)
    {
        sum += logs[i].op_time;
    }
    return sum;
}
