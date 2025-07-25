//------------------------------------------------------------------------------------------------------
// ARM CMSIS and Software Drivers
// 1)Input from 8-bit switch and output to LEDs
// 2)Input characters from keyboard (UART) and output to the terminal
// 3)A counter is incremented every second and displayed on the 7-segment display
//------------------------------------------------------------------------------------------------------

#include "EDK_CM0.h" 
#include "core_cm0.h"
#include "edk_driver.h"
#include <stdint.h>

#define Timer_Interrput_Frequency           1
#define System_Tick_Frequency               50000000
#define Timer_Prescaler                     1 //Timer Prescaler, options are: 256, 16, 1 
#define Timer_Load_Value                    (System_Tick_Frequency/Timer_Interrput_Frequency/Timer_Prescaler)

static char dig1, dig2, dig3, dig4;

void UART_ISR(void)
{
    char c;
    uart_read_byte(&c);
    uart_write_byte(c);

    return;
}

void Timer_ISR(void)
{
    dig4++;
    
    if(dig4==10) {
        dig4=0;
        dig3++;
        if (dig3==10) {
            dig3=0;
            dig2++;
            if (dig2==10){
                dig2=0;
                dig1++;
            }
        }
    }
    
    seven_seg_write(dig1, dig2, dig3, dig4, 0);
    timer_irq_clear();

    return;
}

void GPIO_ISR(void)
{
    return;
}

int main(void)
{
    seven_seg_write(0, 0, 0, 0, 0);
    uart_init(B115200, 0);
    GPIO_init(1 << 3 | 1 << 2 | 1 << 1 | 1 << 0);
    struct pt point1 = {.x = 0, .y = 0};
    struct pt point2 = {.x = 10, .y = 0};
    struct pt point3 = {.x = 0, .y = 0};
    struct pt point4 = {.x = 0, .y = 10};

    VGA_set_resolution(VGA_8x8);
    VGA_plot_line(point1, point2, 0xF0, HORIZONTAL);
    VGA_plot_line(point3, point4, 0x0F, VERTICAL);

    NVIC_SetPriority(Timer_IRQn, 0x00);
    NVIC_SetPriority(UART_IRQn, 0x40);
    NVIC_EnableIRQ(Timer_IRQn);
    NVIC_EnableIRQ(UART_IRQn);
    timer_init(Timer_Load_Value, Timer_Prescaler, 1);
    timer_enable();
	
    while(1)
    { 
        GPIO_write_pin(0, GPIO_read_pin(4));
        GPIO_write_pin(1, GPIO_read_pin(5));
        GPIO_write_pin(2, GPIO_read_pin(6));
        GPIO_write_pin(3, GPIO_read_pin(7));
    }
}

