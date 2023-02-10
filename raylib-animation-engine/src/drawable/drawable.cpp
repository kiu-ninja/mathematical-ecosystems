#include "drawables.hpp"
#include "drawing_functions.hpp"

using namespace Drawables;

TimedScene* Drawable::move_to(const Vector2 &destination) {
    return Interpolate::interpolate<Vector2>(
        &this->position, 
        destination,
        Interpolate::Mode::CUBIC, 
        Interpolate::Behavior::STATIC
    );
}

TimedScene* Drawable::translate(const Vector2 &offset) {
    return Interpolate::interpolate<Vector2>(
        &this->position, 
        offset, 
        Interpolate::Mode::CUBIC, 
        Interpolate::Behavior::RELATIVE_DELTA
    );
}

TimedScene* Drawable::scale(const Vector2 &factor) {
    return Interpolate::interpolate<Vector2>(
        &this->dimensions, 
        factor, 
        Interpolate::Mode::CUBIC, 
        Interpolate::Behavior::RELATIVE_FACTOR
    );
}

TimedScene* Drawable::scale(const float &factor) {
    return scale(Vector2 { factor, factor });
}

Vector2 Drawable::closest_point(Vector2 point) {
    Vector2 delta = point - position;
    Vector2 ratio = normalized(delta / dimensions * 2);

    if (abs(ratio.x) <= abs(ratio.y)) {
        return position + Vector2 {
            ratio.x / abs(ratio.y) * dimensions.x / 2,
            dimensions.y / 2 * (delta.y >= 0 ? 1 : -1)
        };
    } else {
        return position + Vector2 {
            dimensions.x / 2 * (delta.x >= 0 ? 1 : -1),
            ratio.y / abs(ratio.x) * dimensions.y / 2
        };
    }
}