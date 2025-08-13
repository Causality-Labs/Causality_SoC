#include <stdint.h>
#include "timer.h"


//4-bits Control register: [0]: timer enable, [1] mode (free-run or reload) [2]: prescaler
void timer_init(uint32_t load_value, uint32_t prescale, uint32_t mode)
{
    int control;
    int prescale_bits;

    if (prescale==16)
        prescale_bits=0x01; 
    else
        prescale_bits=0x00; 

    control = ((prescale_bits << 4) | (mode << 1));
    TIMER->INITVALUE = load_value;
    TIMER->CLEAR = 1;
    TIMER->CONTROL = control;

    return;
}

// timer enable
void timer_enable(void)
{
    int control;
    control = TIMER->CONTROL;
    control = control | 0x01;
    TIMER->CONTROL = control;

    return;
}

// clear interrupt request from timer
void timer_irq_clear(void)
{
    TIMER->CLEAR = 0x01;

    return;
}

uint32_t timer_curr_val(void)
{
    return TIMER->CURVALUE;
}
