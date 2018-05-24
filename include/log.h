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
    FP32 quantity; // Litres of gas purchased
    FP32 price;    // Total price of purchase
    BOOLEAN paid_cash;
    INT32U account_num
} LOG_T;

BOOLEAN log_put(INT8U hour, INT8U min, INT8U sec, GAS_T product, FP32 quantity, FP32 price, BOOLEAN paid_cash, INT32U account_num);
LOG_T log_get(INT8U log_number);
INT8U log_capacity();

#endif
