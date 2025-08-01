#include "EDK_CM0.h"
#include <string.h>
#include "edk_driver.h"
#include "edk_api.h"
#include "core_cm0.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"

int8_t random(char min, char max)
{
    int8_t i;
    i = (int8_t)timer_curr_val();

    if (i <= 0)
        i = min;

    while (i < min)
        i = 2 * i;

    while (i > max)
        i = i >> 2;

    return i;
}

void delay(int value)
{
    for (int i = 0; i < value; i++) {}
}

void plot_target(targ target, uint8_t colour)
{
    struct pt end_pt =
    {
        .x = target.point.x + 2,
        .y = target.point.y + 2
    };

    plot_hor_line(target.point, end_pt, colour);
    return;
}

void plot_snake(Snake snake)
{
    for (int i = 0; i < snake.node; i++)
    {
        VGA_plot_pixel(snake.point[i], snake.colour);
        VGA_plot_pixel(snake.point[snake.node - 1], BLACK);
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