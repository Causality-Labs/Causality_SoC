//--------------------------------------------------------
// Peripheral driver functions
//--------------------------------------------------------


#include "EDK_CM0.h"
#include <string.h>
#include "edk_driver.h"

//---------------------------------------------
// VGA driver function
//---------------------------------------------
static uint8_t scale_by_res(void)
{
    VGA_Resolution_t current_res = VGA_get_resolution();

    switch(current_res) {
        case VGA_2x2:
            return 0;
        case VGA_4x4:
            return 1;
        case VGA_8x8:
            return 2;
        default:
            return 0;
    }
}

void VGA_plot_rect(struct rect rectangle, uint8_t colour)
{
    struct rect scaled_rect =
    {
        .top_left =     { .x = 0,   .y = 0 },
        .top_right =    { .x = 0, .y = 0 },
        .bottom_left =  { .x = 0,   .y = 0 },
        .bottom_right = { .x = 0, .y = 0 },
    };
    uint8_t shift = scale_by_res();

    scaled_rect.top_left.x     = rectangle.top_left.x     >> shift;
    scaled_rect.top_left.y     = rectangle.top_left.y     >> shift;

    scaled_rect.top_right.x    = rectangle.top_right.x    >> shift;
    scaled_rect.top_right.y    = rectangle.top_right.y    >> shift;

    scaled_rect.bottom_left.x  = rectangle.bottom_left.x  >> shift;
    scaled_rect.bottom_left.y  = rectangle.bottom_left.y  >> shift;

    scaled_rect.bottom_right.x = rectangle.bottom_right.x >> shift;
    scaled_rect.bottom_right.y = rectangle.bottom_right.y >> shift;

    plot_hor_line(scaled_rect.top_left, scaled_rect.top_right, colour);
    plot_hor_line(scaled_rect.bottom_left, scaled_rect.bottom_right, colour);

    plot_vert_line(scaled_rect.top_left, scaled_rect.bottom_left, colour);
    plot_vert_line(scaled_rect.top_right, scaled_rect.bottom_right, colour);

    return;
}

void plot_hor_line(struct pt point1, struct pt point2, uint8_t colour)
{
    if (point1.x > point2.x)
        return;

    uint16_t max_x = 100;
    uint16_t max_y = 120;

    VGA_Resolution_t current_res = VGA_get_resolution();
    switch(current_res) {
        case VGA_2x2:
            max_x = 200;
            max_y = 240;
            break;
        case VGA_4x4:
            max_x = 100;
            max_y = 120;
            break;
        case VGA_8x8:
            max_x = 50;
            max_y = 60;
            break;
        default:
            max_x = 100;
            max_y = 120;
            break;
    }

    if (point1.y >= max_y)
        point1.y = max_y - 1;

    uint16_t diff = point2.x - point1.x;
    for(uint16_t i = 0; i <= diff; i++)
    {
        VGA_plot_pixel(point1, colour);
        point1.x++;
        if (point1.x >= max_x)
            point1.x = max_x - 1;
    }

    return;
}

void plot_vert_line(struct pt point1, struct pt point2, uint8_t colour)
{
    if (point1.y > point2.y)
        return;

    uint16_t max_x = 100;
    uint16_t max_y = 120;

    VGA_Resolution_t current_res = VGA_get_resolution();
    switch(current_res) {
        case VGA_2x2:
            max_x = 200;
            max_y = 240;
            break;
        case VGA_4x4:
            max_x = 100;
            max_y = 120;
            break;
        case VGA_8x8:
            max_x = 50;
            max_y = 60;
            break;
        default:
            max_x = 100;
            max_y = 120;
            break;
    }

    if (point1.x >= max_x)
        point1.x = max_x - 1;

    uint16_t diff = point2.y - point1.y;
    for(uint16_t i = 0; i <= diff; i++)
    {
        VGA_plot_pixel(point1, colour);
        point1.y++;
        if (point1.y >= max_y)
            point1.y = max_y - 1;
    }

    return;
}

void VGA_plot_line(struct pt point1, struct pt point2, uint8_t colour, VGA_Line_t Line_type)
{
    switch(Line_type) {
        case HORIZONTAL:
            plot_hor_line(point1, point2, colour);
            break;
        case VERTICAL:
            plot_vert_line(point1, point2, colour);
            break;
        default:
            plot_hor_line(point1, point2, colour);
            break;
    }

    return;
}
//Plot a pixel to the image buffer
void VGA_plot_pixel(struct pt point, uint8_t colour)
{
    VGA_Resolution_t current_res = VGA_get_resolution();

    uint32_t addr;
    int stride;
    uint16_t max_x = 100;
    uint16_t max_y = 120;

    switch(current_res) {
        case VGA_2x2:
            stride = 256;
            max_x = 200;
            max_y = 240;
            break;
        case VGA_4x4:
            stride = 128;
            max_x = 100;
            max_y = 120;
            break;
        case VGA_8x8:
            stride = 64;
            max_x = 50;
            max_y = 60;
            break;
        default:
            stride = 128;
            max_x = 100;
            max_y = 120;
            break;
    }

    if (point.x < 0 || point.x >= max_x || point.y < 0 || point.y > max_y)
        return;

    addr = point.y * stride + point.x;
    *(&(VGA->IMG) + addr) = colour;

    return;
}


void VGA_set_resolution(VGA_Resolution_t mode)
{
    VGA_RESOLUTION_REG = mode;
}

VGA_Resolution_t VGA_get_resolution(void)
{
    int res = 0;
    res = VGA_RESOLUTION_REG;
    return (VGA_Resolution_t)res;
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
