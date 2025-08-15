#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include "core_cm0.h"
#include "SoC_CM0.h"

// VGA Resolution modes enum
typedef enum {
  VGA_2x2 = 0x01,
  VGA_4x4 = 0x02,
  VGA_8x8 = 0x03
} VGA_Resolution_t;

// VGA Line type
typedef enum {
  HORIZONTAL,
  VERTICAL,
} VGA_Line_t;

struct dim
{
  uint16_t max_x;
  uint16_t max_y;
  VGA_Resolution_t res;
};

struct pt
{
  int x;
  int y;
};

struct rect
{
  struct pt top_left;
  struct pt top_right;
  struct pt bottom_left;
  struct pt bottom_right;
};

struct dim VGA_get_dimensions(void);
void VGA_plot_rect(struct rect rectangle, uint8_t colour);
void VGA_set_resolution(VGA_Resolution_t mode);
VGA_Resolution_t VGA_get_resolution(void);
void VGA_plot_pixel(struct pt point, uint8_t colour);
void VGA_plot_line(struct pt point1, struct pt point2, uint8_t colour, VGA_Line_t Line_type);
void VGA_plot_vert_line(struct pt point1, struct pt point2, uint8_t colour);
void VGA_plot_hor_line(struct pt point1, struct pt point2, uint8_t colour);

#endif /* VGA_H */