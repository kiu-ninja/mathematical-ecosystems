#include "drawables.hpp"
#include "drawing_functions.hpp"

using namespace Drawables;

Rectangle GridCell::get_stroke_rect() {
    return get_rect_padded(0);
}

Rectangle GridCell::get_fill_rect() {
    return get_rect_padded(dimensions.x / 10);
}

Color GridCell::get_stroke_col() {
    return Color {
        stroke_col.r,
        stroke_col.g,
        stroke_col.b,
        (unsigned char)(255 * visibility)
    };
}

Color GridCell::get_fill_col() {
    return Color {
        fill_col.r,
        fill_col.g,
        fill_col.b,
        (unsigned char)(255 * alive * visibility)
    };
}

Scene* GridCell::animate_alive(const float &new_alive) {
    return Interpolate::interpolate<float>(
        &this->alive, 
        new_alive, 
        Interpolate::Mode::CUBIC, 
        Interpolate::Behavior::STATIC
    );
}

Scene* GridCell::animate_visibility(const float &new_visibility) {
    return Interpolate::interpolate<float>(
        &this->visibility, 
        new_visibility, 
        Interpolate::Mode::CUBIC, 
        Interpolate::Behavior::STATIC
    );
}

void GridCell::draw() {
    if (visibility > 0)
        draw_rectangle_circle_bounded(
            this->get_stroke_rect(), 
            get_rectangle_occluding_circle_offset(Rectangle {
                position.x, position.y, dimensions.x, dimensions.y
            }, circle_offset, visibility), 
            this->get_stroke_col()
        );
    if (alive * visibility > 0)
        DrawRectangleRec(
            this->get_fill_rect(), 
            this->get_fill_col()
        );
}