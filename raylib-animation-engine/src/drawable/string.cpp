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

Scene* String::write(const std::string &target_text) {
    struct DrawableStringWirteScene: public Scene { using Scene::Scene;
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
        
        Scene* begin() override {
            ((String *)this->object)->update_text(this->target);
            ((String *)this->object)->t = 0;
            return this;
        }

        void act() override {
            float t = get_scene_controller()->get_t();
            ((String *)this->object)->t = Easing::cubic(0, 1, t);
        }

        Scene* finish() override {
            return {};
        }
    };

    DrawableStringWirteScene* res = new DrawableStringWirteScene();
    res->set_target(target_text);
    res->set_object(this);
    res->set_scene_controller(new TimedSceneController(0.0f, target_text.size() / 10.0f));

    return res;
}

Scene* String::appear() {
    return Interpolate::interpolate<float>(&this->alpha, 1, Interpolate::Mode::CUBIC_CUBIC, Interpolate::Behavior::STATIC);
}

Scene* String::disappear() {
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