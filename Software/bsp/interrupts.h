#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "SoC_gpio.h"
#include "SoC_uart.h"
#include "SoC_VGA.h"
#include "SoC_seven_segment.h"
#include "SoC_timer.h"
#include "SoC_api.h"
#include "core_cm0.h"                       /* Cortex-M0 processor and core peripherals           */
#include <stdint.h>

#define NVIC_INT_ENABLE     0xE000E100

void UART_ISR(void);
void Timer_ISR(void);
void GPIO_ISR(void);

#endif /* INTERRUPTS_H */
