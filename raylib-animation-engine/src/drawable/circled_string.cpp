#include "drawables.hpp"
#include "drawing_functions.hpp"

using namespace Drawables;

void CircledString::draw() {
    if (t > 0 && alpha > 0) {
        DrawCircleV(position, length(this->get_pos() - position) + font_size / 3, Color{ col.r, col.g, col.b, (unsigned char)(clamp(alpha * 255, 0.0f, 255.0f)) });
        DrawCircleV(position, length(this->get_pos() - position) + font_size / 3 - 2, Color{ 0, 0, 0, (unsigned char)(clamp(alpha * 255, 0.0f, 255.0f)) });
        DrawTextEx(
            this->font, 
            this->get_string().c_str(), 
            this->get_pos(), 
            this->font_size, 
            0, 
            Color{ col.r, col.g, col.b, (unsigned char)(clamp(alpha * 255, 0.0f, 255.0f)) }
        );
    }
}