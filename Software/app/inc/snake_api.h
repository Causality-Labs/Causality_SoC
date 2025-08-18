#ifndef SNAKE_API_H
#define SNAKE_API_H

#include <stdint.h>
#include "SoC_VGA.h"
#include "SoC_api.h"

#define N 200

#define HEAD 0

typedef enum {
    SNAKE_UP,
    SNAKE_DOWN,
    SNAKE_LEFT,
    SNAKE_RIGHT
} Snake_direction_t;

typedef struct {
    struct pt point;
    int reach;
} targ;


typedef struct {
    struct pt point[N];
    int node;
    uint8_t colour;
    Snake_direction_t direction;
} Snake;


void plot_target(targ top_left, uint8_t colour);
void snake_plot(Snake *snake);
void snake_move(Snake *snake);
bool snake_self_collision(Snake *snake);
bool snake_wall_collision(Snake *snake);
void target_gen(targ *target);
bool check_overlap(Snake *snake, targ *target);
bool snake_ate_target(struct pt *p, targ *target);

#endif /* SNAKE_API_H*/
