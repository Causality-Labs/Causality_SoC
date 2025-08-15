#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "core_cm0.h"
#include "SoC_CM0.h"

void timer_init(uint32_t load_value, uint32_t prescale, uint32_t mode);
void timer_enable(void);
void timer_irq_clear(void);
uint32_t timer_curr_val(void);

#endif /* TIMER_H */