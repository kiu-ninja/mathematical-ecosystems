#include "drawables.hpp"
#include "drawing_functions.hpp"

using namespace Drawables;

Arrow* Arrow::from(Drawable* object) {
    this->_from = object;

    return this;
}


Arrow* Arrow::to(Drawable* object) {
    this->_to = object;

    return this;
}

Scene* Arrow::appear() {
    return Interpolate::interpolate<float>(&this->visibility, 1, Interpolate::Mode::CUBIC_CUBIC, Interpolate::Behavior::STATIC);
}

Scene* Arrow::disappear() {
    return Interpolate::interpolate<float>(&this->visibility, 0, Interpolate::Mode::CUBIC_CUBIC, Interpolate::Behavior::STATIC);
}

void Arrow::draw() {
    if (visibility > 0) {
        Vector2 pos = this->_from->closest_point(this->_to->position);
        Vector2 dim = this->_to->closest_point(this->_from->position) - pos;

        std::vector<Vector2> points;
        Vector2 n = normal(normalized(dim));
        points.push_back(pos + weight * n / 2);
        points.push_back(pos - weight * n / 2);
        points.push_back(pos - weight * n / 2 + dim - normal(-n) * head_size);
        points.push_back(pos - weight * n / 2 + dim - normal(-n) * head_size);
        points.push_back(pos - n * head_size / 2 + dim - normal(-n) * head_size);
        points.push_back(pos + dim);
        points.push_back(pos + n * head_size / 2 + dim - normal(-n) * head_size);
        points.push_back(pos + weight * n / 2 + dim - normal(-n) * head_size);

        draw_line_segment_outline(points, visibility, col);
    }
}