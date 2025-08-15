#include "interrupts.h"
#include "SoC_uart.h"
#include "SoC_timer.h"
#include  "SoC_gpio.h"

extern char key;
extern int key_pending;
extern int timer_tick;

void UART_ISR(void)
{
    key = UartGetc();
    key_pending = 1;

    return;
}

void Timer_ISR(void)
{
    if (timer_tick == 0)
        timer_tick = 1;

    timer_irq_clear(); 

    return;
}

void GPIO_ISR(void)
{
    return;
}