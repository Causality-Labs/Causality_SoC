#include <stdint.h>
#include <stdbool.h>
#include "snake_api.h"
#include "SoC_VGA.h"
#include "SoC_api.h"

static bool is_point_in_target(struct pt *p, targ *target);

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

bool snake_ate_target(struct pt *p, targ *target)
{
    return is_point_in_target(p, target);
}

void target_gen(targ *target)
{
    target->point.x = random(2, 97);
    target->point.x = target->point.x - target->point.x % 2;
    delay(111 *  target->point.x);

    target->point.y = random(2, 117);
    target->point.y = target->point.y - target->point.y % 2;

    return;
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

    VGA_plot_hor_line(top_left.point, top_right, colour);
    VGA_plot_hor_line(bottom_left, bottom_right, colour);

    return;
}

bool check_overlap(Snake *snake, targ *target)
{
    for (uint8_t i = 0; i < snake->node; i++)
    {
        if (is_point_in_target(&snake->point[i], target))
            return true;
    }

    return false;
}

static bool is_point_in_target(struct pt *p, targ *target)
{
    int px = p->x;
    int py = p->y;
    int tx = target->point.x;
    int ty = target->point.y;

    return (px >= tx && px <= tx + 1 &&
            py >= ty && py <= ty + 1);
}
