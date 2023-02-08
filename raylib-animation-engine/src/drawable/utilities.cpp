#include "drawables.hpp"
#include "drawing_functions.hpp"

using namespace Drawables;

Rectangle Drawables::padded_rectangle(const Rectangle &rect, const float &padding) {
    return Rectangle {rect.x + padding, rect.y + padding, rect.width - padding * 2, rect.height - padding * 2};
}

Rectangle Drawables::centered_rectangle(const Rectangle &rect) {
    return Rectangle { rect.x - rect.width / 2, rect.y - rect.height / 2, rect.width, rect.height };
}

Circle Drawables::get_rectangle_occluding_circle_offset(const Rectangle &rect, const Vector2 &circle_offset, const float &t) {
    Vector2 o{
        rect.x + rect.width * circle_offset.x,
        rect.y + rect.height * circle_offset.y
    };

    float d1 = length_squared(o - Vector2{rect.x, rect.y});
    float d2 = length_squared(o - Vector2{rect.x + rect.width, rect.y});
    float d3 = length_squared(o - Vector2{rect.x, rect.y + rect.height});
    float d4 = length_squared(o - Vector2{rect.x + rect.width, rect.y + rect.height});

    float min_d = std::min(rect.width * std::min(circle_offset.x, 1-circle_offset.x), rect.height * std::min(circle_offset.y, 1-circle_offset.y));
    float max_d = std::sqrtf(std::max(std::max(d1, d2), std::max(d3, d4)));

    return Circle { o - Vector2{rect.width / 2, rect.height / 2}, Easing::linear(min_d, max_d, t) };
}