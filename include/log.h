#ifndef LOG_H_
#define LOG_H_

#include <stdint.h>
#include "emp_type.h"

#define MAX_LOGS 128

typedef enum {GAS_92, GAS_95, GAS_E10} GAS_T;

typedef struct {
    INT8U hour; // Time of purchase
    INT8U min;
    INT8U sec;
    GAS_T product; // Type of gas (92/95/E10)
    INT32U quantity; // Millilitres of gas purchased (to avoid floats)
    INT32U price;    // Total price of purchase
    BOOLEAN paid_cash;
    INT32U account_num
} LOG_T;

BOOLEAN log_put(INT8U hour, INT8U min, INT8U sec, GAS_T product, INT32U quantity, INT32U price, BOOLEAN paid_cash, INT32U account_num);
LOG_T log_get(INT8U log_number);
INT8U log_capacity();
INT32U log_get_cash_sum();
INT32U log_get_acc_sum();

#endif
