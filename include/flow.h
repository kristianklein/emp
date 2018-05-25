#ifndef FLOW_H_
#define FLOW_H_

#include <stdint.h>
#include "emp_type.h"

void flow_init();
void flow_reset();
INT32U flow_get_pulses();
void flow_run_mode(BOOLEAN mode);
void flow_shunt_mode(BOOLEAN mode);
void timer0a_isr();


#endif
