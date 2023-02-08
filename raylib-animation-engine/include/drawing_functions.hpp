// #pragma once

#include <vector>
#include "raylib.h"
#include "circle.hpp"

void draw_grid(const Vector2 &center, const Rectangle &bounds, const float &cell_size, const Color &col);
inline char _get_point_out_bounds_directions(const Vector2 &p, const Rectangle &bounds);
void draw_line_rectangle_bounded(Vector2 a, Vector2 b, const Rectangle &bounds, const Color &color);
void draw_rectangle_circle_bounded(const Rectangle &r, const Circle &c, const Color &col);
void draw_line_segment_outline_bounded(const Rectangle &r, const std::vector<Vector2> &points, const float &t, const Color &col);
void draw_line_segment_outline(const std::vector<Vector2> &points, const float &t, const Color &col);