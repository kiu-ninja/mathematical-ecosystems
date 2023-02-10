#include "drawables.hpp"
#include "drawing_functions.hpp"

using namespace Drawables;

Rectangle Rect::get_rect() {
    return get_rect_padded(0);
}

Rectangle Rect::get_rect_padded(float padding) {
    return padded_rectangle(centered_rectangle(Rectangle {
        position.x, position.y, dimensions.x, dimensions.y
    }), padding);
}

void Rect::draw() {
    draw_rectangle_circle_bounded(
        this->get_rect(),
        get_rectangle_occluding_circle_offset(Rectangle {
            position.x, position.y, dimensions.x, dimensions.y
        }, circle_offset, visibility), 
        WHITE
    );
}