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

Circle Rect::get_occluder() {
    return get_rectangle_occluding_circle_offset(Rectangle {
        position.x, position.y, dimensions.x, dimensions.y
    }, circle_offset, visibility);
}

void Rect::draw() {
    draw_rectangle_circle_bounded(
        this->get_rect(),
        this->get_occluder(), 
        WHITE
    );
}