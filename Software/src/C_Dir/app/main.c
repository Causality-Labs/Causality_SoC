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
#include <stdbool.h>

static int score;
static int gamespeed;
static int speed_table[10]= {6, 9, 12, 15, 20, 25, 30, 35, 40, 100};
static int pause;
static int snake_has_moved = 0;
static int snake_index = 0;

volatile char key = 0;
volatile int key_pending = 0;
volatile int timer_tick = 0;

targ target;
Snake snake;

void Game_Init(void);
void Game_Update(void);
//void Game_Over(void);
//void Game_Close(void);
void target_gen(void);
bool check_overlap(void);
bool is_point_in_target(struct pt p, targ *target);

void UART_ISR(void)
{
    key = UartGetc();
    key_pending = 1;

    return;
}

void Timer_ISR(void)
{
    //seven_seg_write(0, 0, 0, (snake_index++) % 10, 1);
    if (timer_tick == 0) {
        seven_seg_write(0,0,0,(snake_index++) % 10, 1);
        timer_tick = 1;
    }

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
            Game_Update();
            timer_tick = 0;
        }
    }
}

static bool is_point_in_target(struct pt p, targ *target)
{
    int px = p.x;
    int py = p.y;
    int tx = target->point.x;
    int ty = target->point.y;

    return (px >= tx && px <= tx + 1 &&
            py >= ty && py <= ty + 1);
}

void target_gen(void)
{
    target.point.x = random(2, 97);
    target.point.x = target.point.x - target.point.x % 2;
    delay(111 *  target.point.x);

    target.point.y = random(2, 117);
    target.point.y = target.point.y - target.point.y % 2;

    return;
}

bool check_overlap(void)
{
    for (uint8_t i = 0; i < snake.node; i++)
    {
        if (is_point_in_target(snake.point[i], &target))
            return true;
    }

    return false;
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
    timer_tick = 0;
    start_interrupts();

    return;
}

void Game_Update(void)
{
    if (pause == 1)
        return;

    if (target.reach == 1) {
        do {
            target_gen();
        } while (check_overlap());

        target.reach = 0;
        plot_target(target, GREEN);
    }

    snake_move(&snake);

    if (is_point_in_target(snake.point[HEAD], &target)) {
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
    }

    snake_plot(&snake);

    snake_has_moved = 1;
    return;
}
#if 0
void Game_Update(void)
{
    void Game_Update(void)
    {
        int overlap;

        if (pause == 0)
        {
            if (target.reach == 1)
            {
                do {
                    overlap = 0;
                    target_gen();
                    for (i = 0; i < snake.node; i++) {
                        if (snake.x[i] == target.x && snake.y[i] == target.y) {
                            overlap = 1;
                            break;
                        }
                    }
                } while (overlap == 1);

                // Draw the new target
                //rectangle(target.x, target.y, target.x + 2, target.y + 2, GREEN);
                //plot_hor_line(target.point, target.point,GREEN);
            }

            // Shift the snake body
            for (i = snake.node - 1; i > 0; i--) {
                snake.x[i] = snake.x[i - 1];
                snake.y[i] = snake.y[i - 1];
            }

            // Move the snake head
            switch (snake.direction) {
                case 1: 
                    snake.x[0] += 2;
                    break; // RIGHT
                case 2:
                    snake.x[0] -= 2;
                    break; // LEFT
                case 3:
                    snake.y[0] -= 2;
                    break; // UP
                case 4:
                    snake.y[0] += 2;
                    break; // DOWN
            }

            // Detect if the snake reached the target
            if (snake.x[0] == target.x && snake.y[0] == target.y) {
                rectangle(target.x, target.y, target.x + 2, target.y + 2, BLACK); // Clear old target

                snake.x[snake.node] = -10;
                snake.y[snake.node] = -10;
                snake.node++;
                target.reach = 1;
                score++;

                if (score <= 10)
                    gamespeed = speed_table[score];

                timer_init((Timer_Load_Value_For_One_Sec / gamespeed), Timer_Prescaler, 1);
                timer_enable();

                write_LED(score);
                printf("\nScore=%d\n", score);
            }

            // Check for self-collision
            for (i = 3; i < snake.node; i++) {
                if (snake.x[i] == snake.x[0] && snake.y[i] == snake.y[0]) {
                    if (GameOver() == 0)
                        Game_Close();
                    else
                        Game_Init();
                    return;
                }
            }

            // Check for wall collision
            if (snake.x[0] < left_boundary + boundary_thick || snake.x[0] >= right_boundary ||
                snake.y[0] < top_boundary + boundary_thick || snake.y[0] >= bottom_boundary)
            {
                if (GameOver() == 0)
                    Game_Close();
                else
                    Game_Init();
                return;
            }

            // Draw updated snake
            for (i = 0; i < snake.node; i++)
                rectangle(snake.x[i], snake.y[i], snake.x[i] + 2, snake.y[i] + 2, RED);

            // Clear tail (last segment)
            rectangle(snake.x[snake.node - 1], snake.y[snake.node - 1],
                    snake.x[snake.node - 1] + 2, snake.y[snake.node - 1] + 2, BLACK);
        }

        snake_has_moved = 1;

        Display_Int_Times();
    }


    return;
}
#endif
