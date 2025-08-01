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

//Maximum snake length
#define N 200

static int score;
static int gamespeed;
static int speed_table[10]= {6, 9, 12, 15, 20, 25, 30, 35, 40, 100};
static int pause;
static int snake_has_moved;

volatile char key = 0;
volatile int key_pending = 0;
volatile int timer_tick = 0;

typedef struct {
    int x;
    int y;
    int reach;
} targ;


typedef struct {
    int x[N];
    int y[N];
    int node;
    int direction;
} Snake;

targ target;
Snake snake;

void Game_Init(void);

void UART_ISR(void)
{
    key = UartGetc();
    key_pending = 1;

    return;
}

void Timer_ISR(void)
{
    timer_tick = 1;
    timer_irq_clear(); 

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
        if (key_pending) {
            key_pending = 0;

            if (key == PAUSE) {
                pause = !pause;
                if (pause)
                    NVIC_DisableIRQ(Timer_IRQn);
                else
                    NVIC_EnableIRQ(Timer_IRQn);
            }

            if (snake_has_moved) {
                switch (key) {
                    case UP:
                        if (snake.direction != 4)
                            snake.direction = 3;
                        break;
                    case RIGHT:
                        if (snake.direction != 2)
                            snake.direction = 1;
                        break;
                    case LEFT:
                        if (snake.direction != 1)
                            snake.direction = 2;
                        break;
                    case DOWN:
                        if (snake.direction != 3)
                            snake.direction = 4;
                        break;
                    default:
                        break;
                }

                snake_has_moved = 0;
            }
        }
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

    score = 0;
    gamespeed=speed_table[score];

    timer_init((Timer_Load_Value_For_One_Sec/gamespeed), Timer_Prescaler, 1);
    timer_enable();

    target.reach = 1;
    snake.direction = 1;

    snake.x[0] = 60;
    snake.y[0] = 80;
    snake.x[1] = 62;
    snake.y[1] = 80;
    snake.node = 4;
    pause = 0;

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

void Game_Update(void)
{

    return;
}