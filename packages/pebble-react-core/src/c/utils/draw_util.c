#include "draw_util.h"

#include <math.h>

GPoint draw_util_rotate_point(GPoint center, GPoint point, int16_t angle)
{
  double fangle = (double)angle * 3.14159265 / 180;

  const int16_t cx = center.x;
  const int16_t cy = center.y;

  const int16_t rx = ((point.x - cx) * cos(fangle) + (point.y - cy) * sin(fangle)) + cx;
  const int16_t ry = (-(point.x - cx) * sin(fangle) + (point.y - cy) * cos(fangle)) + cy;

  return (GPoint){rx, ry};
}