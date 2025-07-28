//------------------------------------------------------------------------------------------------------
// ARM CMSIS and Software Drivers
// 1)Input from 8-bit switch and output to LEDs
// 2)Input characters from keyboard (UART) and output to the terminal
// 3)A counter is incremented every second and displayed on the 7-segment display
//------------------------------------------------------------------------------------------------------

#include "EDK_CM0.h" 
#include "core_cm0.h"
#include "edk_driver.h"
#include <stdint.h>

#define Timer_Interrput_Frequency           1
#define System_Tick_Frequency               50000000
#define Timer_Prescaler                     1 //Timer Prescaler, options are: 256, 16, 1 
#define Timer_Load_Value                    (System_Tick_Frequency/Timer_Interrput_Frequency/Timer_Prescaler)

#define RED 0xE0
#define GREEN 0x1C
#define BLUE 0x03
#define WHITE 0xFF
#define BLACK 0x00

static char dig1;
static char dig2;
static char dig3;
static char dig4;

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
    VGA_set_resolution(VGA_2x2);

    struct rect rectangle =
    {
        .top_left =     { .x = 0,   .y = 0 },
        .top_right =    { .x = 199, .y = 0 },
        .bottom_left =  { .x = 0,   .y = 239 },
        .bottom_right = { .x = 199, .y = 239 },
    };

    VGA_plot_rect(rectangle, RED);

    while(1)
    { 

    }
}

