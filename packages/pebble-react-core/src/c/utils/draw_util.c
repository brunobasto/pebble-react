#include "draw_util.h"

#include <math.h>

GPoint draw_util_rotate_point(GPoint center, GPoint point, int16_t angle)
{
  const int16_t cx = center.x;
  const int16_t cy = center.y;
  const int32_t triangleAngle = DEG_TO_TRIGANGLE(angle);

  const int16_t rx = ((point.x - cx) * cos_lookup(triangleAngle) / TRIG_MAX_RATIO + (point.y - cy) * sin_lookup(triangleAngle) / TRIG_MAX_RATIO) + cx;
  const int16_t ry = (-(point.x - cx) * sin_lookup(triangleAngle) / TRIG_MAX_RATIO + (point.y - cy) * cos_lookup(triangleAngle) / TRIG_MAX_RATIO) + cy;

  return (GPoint){rx, ry};
}

GRect draw_util_rect_fitting_points(GPoint *points, uint8_t num_points)
{
  int16_t minX = 144, maxX = 0, minY = 168, maxY = 0;

  for (uint8_t i = 0; i < num_points; i++)
  {
    if (points[i].x < minX)
      minX = points[i].x;
    if (points[i].x > maxX)
      maxX = points[i].x;
    if (points[i].y < minY)
      minY = points[i].y;
    if (points[i].y > maxY)
      maxY = points[i].y;
  }

  return GRect(minX, minY, maxX - minX, maxY - minY);
}