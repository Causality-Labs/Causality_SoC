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

static int score;
static int gamespeed;
static int speed_table[10]= {6, 9, 12, 15, 20, 25, 30, 35, 40, 100};
char key;

void Game_Init(void);

void UART_ISR(void)
{
    //key = UartGetc();

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
    Game_Init();

    while(1)
    { 

    }
}

void Game_Init(void)
{
    struct rect rectangle =
    {
        .top_left =     { .x = 0,   .y = 0 },
        .top_right =    { .x = 99, .y = 0 },
        .bottom_left =  { .x = 0,   .y = 119 },
        .bottom_right = { .x = 99, .y = 119 },
    };
    VGA_plot_rect(rectangle, BLUE);

    timer_init((Timer_Load_Value_For_One_Sec/gamespeed), Timer_Prescaler, 1);
    timer_enable();

    printf("\n------- Snake Game --------");
    printf("\nCentre btn ..... hard reset");
    printf("\nKeyboard r ..... soft reset");
    printf("\nKeyboard w ........ move up");
    printf("\nKeyboard s ...... move down");
    printf("\nKeyboard a ...... move left");
    printf("\nKeyboard d ..... move right");
    printf("\nKeyboard space ...... pause");
    printf("\n---------------------------");  
    printf("\nTo run the game, make sure:");
    printf("\n*UART terminal is activated");
    printf("\n*UART baud rare:  115200 bps");
    printf("\n*Keyboard is in lower case");
    printf("\n---------------------------");
    printf("\nPress any key to start\n");

    while(KBHIT() == 0) 
    {
        // Do nothing.
    }

    printf("\nScore = %d\n", score);
    start_interrupts();

    return;
}
