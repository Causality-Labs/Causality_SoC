#include <string.h>
#include <stdbool.h>
#include "SoC_CM0.h" 
#include "SoC_gpio.h"
#include "SoC_uart.h"
#include "SoC_VGA.h"
#include "SoC_seven_segment.h"
#include "SoC_timer.h"
#include "SoC_api.h"
#include "core_cm0.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"

static uint32_t random_state = 1;

void random_init(void)
{
    random_state = timer_curr_val() ^ 0x12345678;
}

int8_t random(char min, char max)
{
    random_state = random_state * 1664525 + 1013904223;
    uint32_t mixed = random_state ^ timer_curr_val();
    uint8_t range = max - min + 1;

    return ((mixed >> 8) % range) + min;

}

void delay(int value)
{
    for (int i = 0; i < value; i++) {}
}


void SoC_init(void)
{
    seven_seg_write(0, 0, 0, 0, 1);
    clear_screen();
    uart_init(B9600, 0);
    VGA_set_resolution(VGA_4x4);
    random_init();
    NVIC_SetPriority(Timer_IRQn, 0x00);
    NVIC_SetPriority(UART_IRQn, 0x40);

    return;
}

void start_interrupts(void)
{
    NVIC_EnableIRQ(Timer_IRQn);
    NVIC_EnableIRQ(UART_IRQn);

    return;
}

void stop_interrupts(void)
{
    NVIC_DisableIRQ(Timer_IRQn);
    NVIC_DisableIRQ(UART_IRQn);

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

void display_score_seven_seg(int score)
{
    int digit0 = score % 10;
    int digit1 = (score / 10) % 10;
    int digit2 = (score / 100) % 10;
    int digit3 = (score / 1000) % 10;

    seven_seg_write(digit3, digit2, digit1, digit0, DECIMAL_MODE);

    return;
}