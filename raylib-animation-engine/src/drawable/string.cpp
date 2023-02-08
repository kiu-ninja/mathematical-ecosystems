#include "drawables.hpp"
#include "drawing_functions.hpp"

using namespace Drawables;

std::string String::get_string() {
    return text.substr(0, text.size() * t);
}

void String::update_text(std::string new_text) {
    text = new_text;

    width = 0;
    height = 1;

    int count = 0;
    for (char c : text) {
        if (c == '\n') {
            count = 0;
            height += 1;
        } else {
            count++;
        }
        width = std::max(width, count);
    }
}

Rectangle String::get_background_rect() {
    Vector2 s = Vector2 { (float)width * font_size / 2, (float)height * font_size };
    Vector2 p = this->position - s / 2;
    return padded_rectangle(Rectangle { p.x, p.y, s.x, s.y }, -font_size / 2);
}

Vector2 String::get_pos() {
    return this->position - Vector2 { (float)width * font_size / 4, (float)height * font_size / 2 };
}

StatelessScene* String::write(const std::string &target_text) {
    struct DrawableStringWirteScene: public StatelessScene { using StatelessScene::StatelessScene;
        Drawable* object;
        std::string initial, target;

        DrawableStringWirteScene* set_object(Drawable* _object) {
            object = _object;
            return this;
        }

        DrawableStringWirteScene* set_target(const std::string _target) {
            target = _target;
            return this;
        }
        
        void start() {
            ((String *)this->object)->update_text(this->target);
            ((String *)this->object)->t = 0;
        }

        void update_state(const float &t) {
            ((String *)this->object)->t = Easing::cubic(0, 1, t);
        }
    };

    DrawableStringWirteScene* res = new DrawableStringWirteScene(1.0f);
    res->set_target(target_text);
    res->set_object(this);
    ((StatelessScene*)res)->set_duration(target_text.size() / 10.0f);

    return res;
}

StatelessScene* String::appear() {
    return Interpolate::interpolate<float>(&this->alpha, 1, Interpolate::Mode::CUBIC_CUBIC, Interpolate::Behavior::STATIC);
}

StatelessScene* String::disappear() {
    return Interpolate::interpolate<float>(&this->alpha, 0, Interpolate::Mode::CUBIC_CUBIC, Interpolate::Behavior::STATIC);
}

void String::draw() {
    if (t > 0 && alpha > 0) {
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