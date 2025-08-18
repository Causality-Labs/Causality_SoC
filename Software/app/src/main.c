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
static int pause;
static int snake_has_moved = 0;

volatile char key = 0;
volatile int key_pending = 0;
volatile int timer_tick = 0;

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
        if (key_pending == 1) {
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
                        if (snake.direction != SNAKE_DOWN)
                            snake.direction = SNAKE_UP;
                        break;
                    case RIGHT:
                        if (snake.direction != SNAKE_LEFT)
                            snake.direction = SNAKE_RIGHT;
                        break;
                    case LEFT:
                        if (snake.direction != SNAKE_RIGHT)
                            snake.direction = SNAKE_LEFT;
                        break;
                    case DOWN:
                        if (snake.direction != SNAKE_UP)
                            snake.direction = SNAKE_DOWN;
                        break;
                    default:
                        break;
                }

                snake_has_moved = 0;
            }
            key_pending = 0;
        }

        if (timer_tick == 1) {
            if (Game_Update() == true)
                timer_tick = 0;
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

    target.reach = 1;
    snake.direction = SNAKE_LEFT;


    snake.point[HEAD].x = 60;
    snake.point[HEAD].y = 80;

    snake.point[HEAD + 1].x = 61;
    snake.point[HEAD + 1].y = 80;

    snake.point[HEAD + 2].x = 62;
    snake.point[HEAD + 2].y = 80;

    snake.point[HEAD + 3].x = 62;
    snake.point[HEAD + 3].y = 80;
    
    snake.colour = RED;
    snake.node = 4;
    pause = 0;

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
    timer_tick = 0;
    start_interrupts();

    return;
}

bool Game_Update(void)
{
    if (pause == 1)
        return true;

    if (target.reach == 1) {
        do {
            target_gen(&target);
        } while (check_overlap(&snake, &target));

        target.reach = 0;
        plot_target(target, GREEN);
    }

    snake_move(&snake);

    if (snake_ate_target(&snake.point[HEAD], &target)) {
        plot_target(target, BLACK);
        snake.point[snake.node].x = -10;
        snake.point[snake.node].y = -10;
        snake.node++;

        snake.point[snake.node].x = -11;
        snake.point[snake.node].y = -10;
        snake.node++;

        snake.point[snake.node].x = -12;
        snake.point[snake.node].y = -10;
        snake.node++;

        snake.point[snake.node].x = -12;
        snake.point[snake.node].y = -10;
        snake.node++;

        target.reach = 1;
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

    snake_has_moved = 1;
    return true;
}

