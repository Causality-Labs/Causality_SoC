#include <stdint.h>
#include "SoC_VGA.h"

struct dim VGA_get_dimensions(void)
{
    struct dim dim;
    dim.res = VGA_get_resolution();

    switch (dim.res) {
        case VGA_2x2:
            dim.max_x = 200;
            dim.max_y = 240;
            break;
        case VGA_4x4:
            dim.max_x = 100;
            dim.max_y = 120;
            break;
        case VGA_8x8:
            dim.max_x = 50;
            dim.max_y = 60;
            break;
        default:
            dim.max_x = 100;
            dim.max_y = 120;
            break;
    }

    return dim;
}

void VGA_plot_rect(struct rect rectangle, uint8_t colour)
{

    VGA_plot_hor_line(rectangle.top_left, rectangle.top_right, colour);
    VGA_plot_hor_line(rectangle.bottom_left, rectangle.bottom_right, colour);

    VGA_plot_vert_line(rectangle.top_left, rectangle.bottom_left, colour);
    VGA_plot_vert_line(rectangle.top_right, rectangle.bottom_right, colour);

    return;
}

void VGA_plot_hor_line(struct pt point1, struct pt point2, uint8_t colour)
{
    if (point1.x > point2.x)
        return;

    struct dim dimension = VGA_get_dimensions();

    if (point1.y >= dimension.max_y)
        point1.y = dimension.max_y - 1;

    uint16_t diff = point2.x - point1.x;
    for(uint16_t i = 0; i <= diff; i++)
    {
        VGA_plot_pixel(point1, colour);
        point1.x++;
        if (point1.x >= dimension.max_x)
            point1.x = dimension.max_x - 1;
    }

    return;
}

void VGA_plot_vert_line(struct pt point1, struct pt point2, uint8_t colour)
{
    if (point1.y > point2.y)
        return;

    struct dim dimension = VGA_get_dimensions();

    if (point1.x >= dimension.max_x)
        point1.x = dimension.max_x - 1;

    uint16_t diff = point2.y - point1.y;
    for(uint16_t i = 0; i <= diff; i++)
    {
        VGA_plot_pixel(point1, colour);
        point1.y++;
        if (point1.y >= dimension.max_y)
            point1.y = dimension.max_y - 1;
    }

    return;
}

void VGA_plot_line(struct pt point1, struct pt point2, uint8_t colour, VGA_Line_t Line_type)
{
    switch(Line_type) {
        case HORIZONTAL:
            VGA_plot_hor_line(point1, point2, colour);
            break;
        case VERTICAL:
            VGA_plot_vert_line(point1, point2, colour);
            break;
        default:
            VGA_plot_hor_line(point1, point2, colour);
            break;
    }

    return;
}

//Plot a pixel to the image buffer
void VGA_plot_pixel(struct pt point, uint8_t colour)
{
    uint32_t addr;
    int stride;

    struct dim dimension = VGA_get_dimensions();
    switch(dimension.res) {
            case VGA_2x2:
            stride = 256;
            break;
        case VGA_4x4:
            stride = 128;
            break;
        case VGA_8x8:
            stride = 64;
            break;
        default:
            stride = 128;
            break;
    }

    if (point.x < 0 || point.x >= dimension.max_x || point.y < 0 || point.y > dimension.max_y)
        return;

    addr = point.y * stride + point.x;
    *(&(VGA->IMG) + addr) = colour;

    return;
}


void VGA_set_resolution(VGA_Resolution_t mode)
{
    AHB_VGA_RESOLUTION_REG = mode;
}

VGA_Resolution_t VGA_get_resolution(void)
{
    int res = 0;
    res = AHB_VGA_RESOLUTION_REG;
    return (VGA_Resolution_t)res;
}