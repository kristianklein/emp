#ifndef INCLUDE_PRICE_H_
#define INCLUDE_PRICE_H_

#include <stdint.h>
#include "emp_type.h"
#include "log.h" // For gas types (GAS_T)

// Note: All prices are in cents

void price_set(GAS_T type, INT16U new_price);
INT16U price_get(GAS_T type);

#endif
