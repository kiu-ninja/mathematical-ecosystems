#include "drawables.hpp"

Vector2 Drawables::Ellipse::closest_point(Vector2 point)
{
    return this->dimensions * normalized(point - this->position);
}

void Drawables::Ellipse::draw()
{
    DrawEllipse(
        this->position.x, 
        this->position.y, 
        this->dimensions.x, 
        this->dimensions.y, 
        fill_col);
}