//------------------------------------------------------------------------------------------------------
// ARM CMSIS and Software Drivers
// 1)Input from 8-bit switch and output to LEDs
// 2)Input characters from keyboard (UART) and output to the terminal
// 3)A counter is incremented every second and displayed on the 7-segment display
//------------------------------------------------------------------------------------------------------

#include "EDK_CM0.h" 
#include "core_cm0.h"
#include "edk_driver.h"
#include "edk_api.h"
#include <stdint.h>



void UART_ISR(void)
{
    return;
}

void Timer_ISR(void)
{
    return;
}

void GPIO_ISR(void)
{
    return;
}

int main(void)
{
    SoC_init();

    while(1)
    { 

    }
}

