#include "EDK_CM0.h"
#include <string.h>
#include "edk_driver.h"
#include "edk_api.h"
#include "core_cm0.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"

void delay(int value)
{
    for (int i = 0; i < value; i++) {}
}

void SoC_init(void)
{
    seven_seg_write(0, 0, 0, 0, 1);
    clear_screen();
    uart_init(B115200, 0);
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