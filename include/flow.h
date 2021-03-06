#ifndef FLOW_H_
#define FLOW_H_

#include <stdint.h>
#include "emp_type.h"

#define PULSES_PER_100ML 112.5

void flow_init();
void flow_reset();
INT32U flow_get_pulses();
INT32U flow_get_milliliters();
void flow_run_mode(BOOLEAN mode);
void flow_shunt_mode(BOOLEAN mode);
void timer0a_isr();


#endif
