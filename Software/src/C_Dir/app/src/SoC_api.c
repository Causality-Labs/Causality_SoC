#include <string.h>
#include <stdbool.h>
#include "core_cm0.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"

#include "SoC_CM0.h" 
#include "SoC_gpio.h"
#include "SoC_uart.h"
#include "SoC_VGA.h"
#include "SoC_seven_segment.h"
#include "SoC_timer.h"

#include "SoC_api.h"

int8_t random(char min, char max)
{
    uint8_t t = (uint8_t)timer_curr_val();
    uint8_t range = max - min + 1;
    return (t % range) + min;
}

void delay(int value)
{
    for (int i = 0; i < value; i++) {}
}

void plot_target(targ top_left, uint8_t colour)
{
    struct pt top_right =
    {
        .x = top_left.point.x + 1,
        .y = top_left.point.y
    };

    struct pt bottom_right =
    {
        .x = top_left.point.x + 1,
        .y = top_left.point.y + 1
    };

    struct pt bottom_left =
    {
        .x = top_left.point.x,
        .y = top_left.point.y + 1
    };

    plot_hor_line(top_left.point, top_right, colour);
    plot_hor_line(bottom_left, bottom_right, colour);

    return;
}

bool snake_wall_collision(Snake *snake)
{
    struct dim screen = VGA_get_dimensions();
    
    int head_x = snake->point[HEAD].x;
    int head_y = snake->point[HEAD].y;

    // Right wall
    if (head_x >= screen.max_x - 1)
        return true;

    // Left Wall
    if (head_x <= 0)
        return true;

    // Bottom wall
    if (head_y >= screen.max_y - 1)
        return true;

    // Top Wall
    if (head_y <= 0)
        return true;

    return false;
}

bool snake_self_collision(Snake *snake)
{
    // Check if head collides with any body segment
    // Start from index 1 (skip the head itself) and check all body segments
    for (int i = 1; i < snake->node; i++) {
        if (snake->point[HEAD].x == snake->point[i].x && 
            snake->point[HEAD].y == snake->point[i].y)
            return true;
    }
    return false;
}

void snake_plot(Snake *snake)
{
    for (int i = 0; i < snake->node; i++)
    {
        VGA_plot_pixel(snake->point[i], snake->colour);
    }

    VGA_plot_pixel(snake->point[snake->node - 1], BLACK);

    return;
}

void snake_move(Snake *snake)
{
    for (uint8_t i = snake->node - 1; i > 0; i--)
    {
        snake->point[i].x = snake->point[i - 1].x;
        snake->point[i].y = snake->point[i - 1].y;
    }

    switch (snake->direction)
    {
        case SNAKE_RIGHT:
            snake->point[HEAD].x += 1;
            break;
        case SNAKE_LEFT:
            snake->point[HEAD].x -= 1;
            break;
        case SNAKE_DOWN:
            snake->point[HEAD].y += 1;
            break;
        case SNAKE_UP:
            snake->point[HEAD].y -= 1;
            break;
        default:
            snake->point[HEAD].x -= 1;
            break;
    }

    return;
}

void SoC_init(void)
{
    seven_seg_write(0, 0, 0, 0, 1);
    clear_screen();
    uart_init(B9600, 0);
    VGA_set_resolution(VGA_4x4);
    NVIC_SetPriority(Timer_IRQn, 0x00);
    NVIC_SetPriority(UART_IRQn, 0x40);
    // SCB -> SCR = 1<1;

    return;
}

void start_interrupts(void)
{
    NVIC_EnableIRQ(Timer_IRQn);
    NVIC_EnableIRQ(UART_IRQn);

    return;
}

void clear_screen(void)
{
    struct dim dimension = VGA_get_dimensions();
    struct pt point;

    for (point.y = 0; point.y < dimension.max_y; point.y++) {
        for (point.x = 0; point.x < dimension.max_x; point.x++) {
            VGA_plot_pixel(point, BLACK);
        }
    }

    return;
}

void clear_console(void)
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

    return;
}