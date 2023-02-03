#pragma once
#include "raylib.h"
#include "random.hpp"
#include "easing.hpp"

struct Circle {
    Vector2 o;
    float r;
};

struct DrawableRectangle {
    Rectangle rect;
    float t = 0;
    Vector2 circle_offset;

    DrawableRectangle() {}
    DrawableRectangle(Rectangle _rect) : rect(_rect) {
        circle_offset = Vector2 { randf(), randf() };
    }

    Rectangle get_rect() {
        return get_rect_padded(0);
    }

    Rectangle get_rect_padded(float padding) {
        float w = rect.width - padding * 2;
        float h = rect.height - padding * 2;
        return Rectangle {
            rect.x - w / 2,
            rect.y - h / 2,
            w,
            h
        };
    }

    Circle get_occluder() {
        Vector2 o{
            rect.x + rect.width * circle_offset.x,
            rect.y + rect.height * circle_offset.y
        };

        float d1 = length_squared(o - Vector2{rect.x, rect.y});
        float d2 = length_squared(o - Vector2{rect.x + rect.width, rect.y});
        float d3 = length_squared(o - Vector2{rect.x, rect.y + rect.height});
        float d4 = length_squared(o - Vector2{rect.x + rect.width, rect.y + rect.height});

        float min_d = 0;//. * std::min(rect.width * std::min(circle_offset.x, 1-circle_offset.x), rect.height * std::min(circle_offset.y, 1-circle_offset.y));
        float max_d = std::sqrtf(std::max(std::max(d1, d2), std::max(d3, d4)));

        return Circle { o - Vector2{rect.width / 2, rect.height / 2}, Easing::linear(min_d, max_d, t) };
    }
};


class DrawableString {
private:
    std::string text;

    int width, height;

public:
    Font font;
    int font_size;
    float t = 0;

    DrawableString() {}
    DrawableString(std::string _text) {
        update_text(_text);
    }

    std::string get_string() {
        return text.substr(0, text.size() * t);
    }

    void update_text(std::string new_text) {
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

    Vector2 get_pos_centered_at(const Vector2 &center) {
        return center - Vector2 { (float)width * font_size / 4, (float)height * font_size / 2 };
    }
};