#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "SoC_CM0.h"
#include "SoC_gpio.h"
#include "SoC_uart.h"
#include "SoC_VGA.h"
#include "SoC_seven_segment.h"
#include "SoC_timer.h"
#include "SoC_api.h"

#include "core_cm0.h"
#include "snake_api.h"

static int score;
static int gamespeed;
static int speed_table[10]= {6, 9, 12, 15, 20, 25, 30, 35, 40, 100};
static bool pause = false;
static bool snake_has_moved = false;

volatile char key = 0;
volatile bool key_pending = false;
volatile bool timer_tick = false;

targ target;
Snake snake;

void Game_Init(void);
bool Game_Update(void);
bool Game_Over(void);
void Game_Close(void);

int main(void)
{
    SoC_init();
    Game_Init();

    while(1)
    {
        if (key_pending == true) {
            if (key == PAUSE) {
                pause = !pause;

                if (pause)
                    NVIC_DisableIRQ(Timer_IRQn);
                else
                    NVIC_EnableIRQ(Timer_IRQn);
            }

            if (snake_has_moved) {
                change_snake_direction(&snake, key);
                snake_has_moved = false;
            }
            key_pending = false;
        }

        if (timer_tick == true) {
            if (Game_Update() == true)
                timer_tick = false;
            else
            while (1)
            {
                __WFI(); 
            }

        }
    }
}

bool Game_Over(void)
{
    char check_key;

    stop_interrupts();
    printf("\nGame over\n");
    printf("\nPress 'q' to quit");
    printf("\nPress 'r' to replay");

    while (1)
    {
        while(KBHIT() == 0){ /* Do Noting */}
        check_key = UartGetc();
        if (check_key == RESET)
            return false;
        else if (check_key == QUIT)
            return true;
        else
            printf("\nInvalid input");
    
    }
    return true;
}

void Game_Close(void)
{
    clear_screen();
    clear_console();
    stop_interrupts();

    return;
}

void Game_Init(void)
{
    clear_screen();
    clear_console();

    struct rect rectangle =
    {
        .top_left =     { .x = 0,   .y = 0 },
        .top_right =    { .x = 99, .y = 0 },
        .bottom_left =  { .x = 0,   .y = 119 },
        .bottom_right = { .x = 99, .y = 119 },
    };
    VGA_plot_rect(rectangle, BLUE);

    score = 0;
    display_score_seven_seg(score);
    gamespeed = speed_table[3];

    timer_init((Timer_Load_Value_For_One_Sec/gamespeed), Timer_Prescaler, 1);
    timer_enable();

    init_snake_and_target(&snake, &target);

    pause = false;

    printf("\n------- Snake Game --------");
    printf("\nCenter btn ..... hard reset");
    printf("\nKeyboard r ..... soft reset");
    printf("\nKeyboard w ........ move up");
    printf("\nKeyboard s ...... move down");
    printf("\nKeyboard a ...... move left");
    printf("\nKeyboard d ..... move right");
    printf("\nKeyboard space ...... pause");
    printf("\n---------------------------");  
    printf("\nTo run the game, make sure:");
    printf("\n*UART terminal is activated");
    printf("\n*UART baud rate:  9600 bps");
    printf("\n*Keyboard is in lower case");
    printf("\n---------------------------");
    printf("\nPress any key to start\n");

    while(KBHIT() == 0) 
    {
        // Do nothing.
    }

    printf("\nScore = %d\n", score);
    timer_tick = false;
    start_interrupts();

    return;
}

bool Game_Update(void)
{
    if (pause == true)
        return true;

    if (target_was_eaten(&target)) {
        do {
            target_gen(&target);
        } while (check_overlap(&snake, &target));

        display_new_target(&target);
    }

    snake_move(&snake);

    if (snake_ate_target(&snake.point[HEAD], &target)) {
        remove_old_target(&target);
        add_new_snake_node(&snake);

        score++;

        printf("\nScore = %d\n", score);
        display_score_seven_seg(score);
    }

    if (snake_self_collision(&snake)) {
        if (Game_Over() == true) {
            Game_Close();
            return false;
        }
        else {
            Game_Init();
            return true;
        }
    }

    if (snake_wall_collision(&snake)) {
        if (Game_Over() == true) {
            Game_Close();
            return false;
        }
        else {
            Game_Init();
            return true;
        }
    }

    snake_plot(&snake);

    snake_has_moved = true;
    return true;
}

