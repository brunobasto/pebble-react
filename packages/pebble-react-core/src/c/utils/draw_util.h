#include <pebble.h>

GPoint draw_util_rotate_point(GPoint center, GPoint point, int16_t angle);

GRect draw_util_rect_fitting_points(GPoint *points, uint8_t num_points);