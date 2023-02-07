// #pragma once

#include <cmath>
#include "raylib.h"
#include "circle.hpp"
#include "easing.hpp"
#include "vectors.h"

void draw_grid(const Vector2 &center, const Rectangle &bounds, const float &cell_size, const Color &col) {
    const float half_cell_size = cell_size / 2;

    // Vertical lines
    float starting_x = center.x - half_cell_size - floor((center.x - half_cell_size - bounds.x) / cell_size) * cell_size;
    for (float x = starting_x; x < bounds.x + bounds.width; x += cell_size) {
        DrawLine(x, bounds.y, x, bounds.y + bounds.height, col);
    }

    // Horizontal lines
    float starting_y = center.y - half_cell_size - floor((center.y - half_cell_size - bounds.y) / cell_size) * cell_size;
    for (float y = starting_y; y < bounds.y + bounds.height; y += cell_size) {
        DrawLine(bounds.x, y, bounds.x + bounds.width, y, col);
    }
}

inline char _get_point_out_bounds_directions(const Vector2 &p, const Rectangle &bounds) {
    return (p.x < bounds.x ? 1 : 0) |
           (p.x > bounds.x + bounds.width ? 2 : 0) |
           (p.y < bounds.y ? 4 : 0) |
           (p.y > bounds.y + bounds.height ? 8 : 0);
}

void draw_line_rectangle_bounded(Vector2 a, Vector2 b, const Rectangle &bounds, const Color &color) {
	float x_max = bounds.x + bounds.width;
	float y_max = bounds.y + bounds.height;

	char out_code_1 = _get_point_out_bounds_directions(a, bounds);
	char out_code_2 = _get_point_out_bounds_directions(b, bounds);

	while (true) {
		if (!(out_code_1 | out_code_2)) {
			DrawLineV(a, b, color);
			break;
		} else if (out_code_1 & out_code_2) {
            // If both points are out of bounds in the same direction
			break;
		} else {
			char out_code_out = std::max(out_code_1, out_code_2);

			float x, y;

            if (out_code_out & 12) {
                if (out_code_out & 8) y = y_max;     // Above
                if (out_code_out & 4) y = bounds.y;  // Below

				x = Easing::linear(a.x, b.x, (y - a.y) / (b.y - a.y));
            } else if (out_code_out & 3) {
                if (out_code_out & 2) x = x_max;     // Right
                if (out_code_out & 1) x = bounds.x;  // Left

                y = Easing::linear(a.y, b.y, (x - a.x) / (b.x - a.x));
            }

			if (out_code_out == out_code_1) {
                a = {x, y};
				out_code_1 = _get_point_out_bounds_directions(a, bounds);
			} else {
                b = {x, y};
				out_code_2 = _get_point_out_bounds_directions(b, bounds);
			}
		}
	}
}

void draw_line_circle_bounded(const Vector2 &a, const Vector2 &b, const Circle &c, const Color &col) {
    Vector2 ab = b - a;
    Vector2 ao = c.o - a;
    Vector2 bo = c.o - b;
    Vector2 ab_normalized = normalized(ab);
    float d = std::abs(dot(normal(ab_normalized), ao));

    float r_squared = c.r * c.r;

    bool a_out = length_squared(ao) > r_squared;
    bool b_out = length_squared(bo) > r_squared;
    float distance_to_normal = dot(ab_normalized, ao);

    Vector2 fa = a, fb = b;
    if (c.r >= 0) {
        if (a_out && (!b_out || d < c.r))
            fa = a + ab_normalized * (dot(ab_normalized, ao) - std::sqrt(r_squared - d * d));
        if (b_out && (!a_out || d < c.r))
            fb = b - ab_normalized * (dot(-ab_normalized, bo) - std::sqrt(r_squared - d * d));
        if (!a_out || !b_out || (d < c.r && distance_to_normal * distance_to_normal < length_squared(ab) && distance_to_normal > 0))
            DrawLineV(fa, fb, col);
    } else {
        if (d >= -c.r) {
            DrawLineV(a, b, col);
        } else {
            if (distance_to_normal * distance_to_normal < length_squared(ab) && distance_to_normal > 0) {
                if (a_out) {
                    DrawLineV(
                        a,
                        a + ab_normalized * (dot(ab_normalized, ao) - std::sqrt(r_squared - d * d)),
                        col
                    );
                }
                if (b_out) {
                    DrawLineV(
                        b - ab_normalized * (dot(-ab_normalized, bo) - std::sqrt(r_squared - d * d)),
                        b,
                        col
                    );
                }
            }
        }
    }
}

void draw_rectangle_circle_bounded(const Rectangle &r, const Circle &c, const Color &col) {
    const float x1 = r.x;
    const float y1 = r.y;
    const float x2 = r.x + r.width;
    const float y2 = r.y + r.height;
    draw_line_circle_bounded({x1, y1}, {x2, y1}, c, col);  //  TOP
    draw_line_circle_bounded({x1, y2}, {x2, y2}, c, col);  //  BOTTOM
    draw_line_circle_bounded({x1, y1}, {x1, y2}, c, col);  //  LEFT
    draw_line_circle_bounded({x2, y1}, {x2, y2}, c, col);  //  RIGHT
}

void draw_line_segment_outline_bounded(const Rectangle &r, const std::vector<Vector2> &points, const float &t, const Color &col) {
    float lengths[points.size()];
    float total_length = 0;
    for (int i = 0; i < points.size(); i++) {
        lengths[i] = length(points[i] - points[(i + 1) % points.size()]);
        total_length += lengths[i];
    }

    total_length *= t;
    for (int i = 0; i < points.size(); i++) {
        if (total_length < lengths[i]) {
            draw_line_rectangle_bounded(points[i], points[i] + (points[(i + 1) % points.size()] - points[i]) * total_length / lengths[i], r, col);
            break;
        }

        draw_line_rectangle_bounded(points[i], points[(i + 1) % points.size()], r, col);
        total_length -= lengths[i];
    }
}

void draw_line_segment_outline(const std::vector<Vector2> &points, const float &t, const Color &col) {
    draw_line_segment_outline_bounded(Rectangle {-10, -10, 1000000, 1000000}, points, t, col);
}