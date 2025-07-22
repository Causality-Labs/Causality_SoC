//--------------------------------------------------------
// Peripheral driver functions
//--------------------------------------------------------


#include "EDK_CM0.h"
#include <string.h>
#include "edk_driver.h"

//---------------------------------------------
// VGA driver function
//---------------------------------------------

//Plot a pixel to the image buffer

void VGA_plot_pixel(int x, int y, int col)
{
    //100x120 resolution
    int addr;
    addr=y*128+x;
    *(&(VGA->IMG)+addr) = col;
}

//---------------------------------------------
// 7 segment display driver function
//---------------------------------------------

void seven_seg_write(char dig1, char dig2, char dig3, char dig4, uint8_t disp_mode)
{
    
    SEVSEG->DIGIT1 = dig4;
    SEVSEG->DIGIT2 = dig3;
    SEVSEG->DIGIT3 = dig2;
    SEVSEG->DIGIT4 = dig1;
    SEVSEG->DISP = disp_mode;

    return;
}

//---------------------------------------------
// Timer driver function
//---------------------------------------------

//Timer initialization
//4-bits Control register: [0]: timer enable, [1] mode (free-run or reload) [2]: prescaler
void timer_init(int load_value, int prescale, int mode)
{
    int control;
    int prescale_bits;

    if (prescale==16)
        prescale_bits=0x01; 
    else
        prescale_bits=0x00; 

    control = ((prescale_bits << 2) | (mode << 1));
    TIMER->INITVALUE = load_value;
    TIMER->CLEAR = 1;
    TIMER->CONTROL = control;

    return;
}

// timer enable
void timer_enable(void)
{
    int control;
    control = TIMER->CONTROL;
    control = control | 0x01;
    TIMER->CONTROL = control;

    return;
}

// clear interrupt request from timer
void timer_irq_clear(void)
{
    TIMER->CLEAR = 0x01;

    return;
}

//---------------------------------------------
// GPIO driver function
//---------------------------------------------
void GPIO_init(uint8_t direction)
{
    GPIO->DIR = direction;
    GPIO->MASK = 0xFF;

    return;
}

// GPIO read
uint8_t GPIO_read_pin(uint8_t pin_number)
{
    return (GPIO->DATA >> pin_number) & 0x01;
}

// GPIO write
void GPIO_write_pin(uint8_t pin_number, uint8_t value)
{
    if (value) {
        GPIO->DATA |= (1 << pin_number);
    } else {
        GPIO->DATA &= ~(1 << pin_number);
    }
}

void uart_init(UART_BaudRate_t baud_rate, uint8_t parity_bit)
{
    UART->BAUD = baud_rate;
    UART->PARITY = parity_bit;
}

void uart_read_byte(char* byte)
{
    *byte = UART->DATA;
}

void uart_write_byte(char byte)
{
    UART->DATA = byte;
}
