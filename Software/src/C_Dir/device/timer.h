#ifndef TIMER_H
#define TIMER_H

#include "core_cm0.h"
#include <stdint.h>

#define AHB_TIMER_BASE      0x52000000

typedef struct
{
  volatile  unsigned int  INITVALUE;
  volatile  unsigned int  CURVALUE;
  volatile  unsigned int  CONTROL;
  volatile  unsigned int  CLEAR;
  volatile  unsigned int  CMP;
  volatile  unsigned int  CAPT;
} TIMER_TypeDef;

#define TIMER           ((TIMER_TypeDef *) AHB_TIMER_BASE )

void timer_init(uint32_t load_value, uint32_t prescale, uint32_t mode);
void timer_enable(void);
void timer_irq_clear(void);
uint32_t timer_curr_val(void);


#endif /* TIMER_H */