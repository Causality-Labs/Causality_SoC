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
#include <stdio.h>

void Game_Init(void);

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
    struct rect rectangle =
    {
        .top_left =     { .x = 0,   .y = 0 },
        .top_right =    { .x = 199, .y = 0 },
        .bottom_left =  { .x = 0,   .y = 239 },
        .bottom_right = { .x = 199, .y = 239 },
    };
    VGA_plot_rect(rectangle, GREEN);
    Game_Init();

    while(1)
    { 

    }
}

void Game_Init(void)
{
    printf("\n\n-------- EDK Demo ---------");
    printf("\n------- Snake Game --------");
    printf("\nCentre btn ..... hard reset");
    printf("\nKeyboard r ..... soft reset");
    printf("\nKeyboard w ........ move up");
    printf("\nKeyboard s ...... move down");
    printf("\nKeyboard a ...... move left");
    printf("\nKeyboard d ..... move right");
    printf("\nKeyboard space ...... pause");
    printf("\n---------------------------");  
    printf("\nTo ran the game, make sure:");
    printf("\n*UART terminal is activated");
    printf("\n*UART baud rare:  19200 bps");
    printf("\n*Keyboard is in lower case");
    printf("\n---------------------------");
    printf("\nPress any key to start\n");

    return;
}
