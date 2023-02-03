#pragma once
#include "raylib.h"
#include "random.hpp"
#include "easing.hpp"
#include "vectors.h"

struct Circle {
    Vector2 o;
    float r;
};


Rectangle centered_padded_rectangle(const Rectangle &rect, const float &padding) {
    float w = rect.width - padding * 2;
    float h = rect.height - padding * 2;
    return Rectangle {
        rect.x - w / 2,
        rect.y - h / 2,
        w,
        h
    };
}

Circle get_rectangle_occluding_circle_offset(const Rectangle &rect, const Vector2 &circle_offset, const float &t) {
    Vector2 o{
        rect.x + rect.width * circle_offset.x,
        rect.y + rect.height * circle_offset.y
    };

    float d1 = length_squared(o - Vector2{rect.x, rect.y});
    float d2 = length_squared(o - Vector2{rect.x + rect.width, rect.y});
    float d3 = length_squared(o - Vector2{rect.x, rect.y + rect.height});
    float d4 = length_squared(o - Vector2{rect.x + rect.width, rect.y + rect.height});

    float min_d = std::min(rect.width * std::min(circle_offset.x, 1-circle_offset.x), rect.height * std::min(circle_offset.y, 1-circle_offset.y));
    float max_d = std::sqrtf(std::max(std::max(d1, d2), std::max(d3, d4)));

    return Circle { o - Vector2{rect.width / 2, rect.height / 2}, Easing::linear(min_d, max_d, t) };
}

namespace Drawable {
    struct Drawable {
        Vector2 position;
        Vector2 dimensions;

        Drawable() {};
        Drawable(Vector2 _position) : position(_position) {}
        Drawable(Vector2 _position, Vector2 _dimensions) : position(_position), dimensions(_dimensions) {};
    };

    template<typename S>
    struct DrawableTransformScene: public Scene<S> { using Scene<S>::Scene;
        Drawable* object;
        Vector2 initial, target;

        DrawableTransformScene* set_object(Drawable* _object) {
            object = _object;
            return this;
        }

        DrawableTransformScene* set_target(const Vector2 &_target) {
            target = _target;
            return this;
        }
    };

    template<typename S>
    Scene<S>* move_to(Drawable* object, const Vector2 &destination) {
        struct MoveTo: public DrawableTransformScene<S> { using DrawableTransformScene<S>::DrawableTransformScene;
            void start() {
                this->initial = this->object->position;
            }

            void update_state(const float &t) {
                this->object->position.x = Easing::cubic(this->initial.x, this->target.x, t);
                this->object->position.y = Easing::cubic(this->initial.y, this->target.y, t);
            }
        };

        MoveTo* res = new MoveTo(1.0f);
        res->set_object(object);
        res->set_target(destination);

        return res;
    }

    template<typename S>
    Scene<S>* translate(Drawable* object, const Vector2 &offset) {
        struct Translate: public DrawableTransformScene<S> { using DrawableTransformScene<S>::DrawableTransformScene;
            Vector2 previous_offset;

            void start() {
                this->initial = this->object->position;
                previous_offset = Vector2 { 0, 0 };
            }

            void update_state(const float &t) {
                this->object->position -= previous_offset;
                previous_offset = Easing::cubic(0, 1, t) * this->target - previous_offset;
                this->object->position += previous_offset;
            }
        };

        Translate* res = new Translate(1.0f);
        res->set_object(object);
        res->set_target(offset);

        return res;
    }

    template<typename S>
    Scene<S>* scale(Drawable* object, const Vector2 &factor) {
        struct Scale: public DrawableTransformScene<S> { using DrawableTransformScene<S>::DrawableTransformScene;
            void start() {
                this->initial = this->object->dimensions;
                this->target = Vector2 {
                    this->object->dimensions.x * this->target.x,
                    this->object->dimensions.y * this->target.y
                };
            }

            void update_state(const float &t) {
                this->object->dimensions.x = Easing::cubic(this->initial.x, this->target.x, t);
                this->object->dimensions.y = Easing::cubic(this->initial.y, this->target.y, t);
            }
        };

        return (new Scale(1.0f))->set_object(object)->set_target(factor);
    }

    template<typename S, typename D>
    std::vector<Scene<S>*> translate_together(std::vector<D*> objects, const Vector2 &offset) {
        std::vector<Scene<S>*> scenes;

        for (D* object : objects) {
            scenes.push_back(translate<S>(object, offset));
        }

        return scenes;
    }

    template<typename S, typename D>
    std::vector<Scene<S>*> scale_together(std::vector<D*> objects, const Vector2 &factor) {
        Vector2 center = Vector2 { 0, 0 };
        for (D* object : objects) {
            center += object->position;
        }
        center = center / objects.size();

        std::vector<Scene<S>*> scenes;

        for (D* object : objects) {
            scenes.push_back(scale<S>(object, factor));
            scenes.push_back(translate<S>(object, Vector2 {
                (center.x - object->position.x) * 2 * factor.x,
                (center.y - object->position.y) * 2 * factor.y
            }));
        }

        std::cout << scenes.size() << "\n";

        return scenes;
    }

    struct Rect: public Drawable {
        Vector2 circle_offset;
        float t = 0;

        Rect() {}
        Rect(const Rectangle &rect) : Drawable(Vector2{rect.x, rect.y}, Vector2{rect.width, rect.height}) {
            circle_offset = Vector2 { randf(), randf() };
        }

        Rectangle get_rect() {
            return get_rect_padded(0);
        }

        Rectangle get_rect_padded(float padding) {
            return centered_padded_rectangle(Rectangle {
                position.x, position.y, dimensions.x, dimensions.y
            }, padding);
        }

        Circle get_occluder() {
            return get_rectangle_occluding_circle_offset(Rectangle {
                position.x, position.y, dimensions.x, dimensions.y
            }, circle_offset, t);
        }
    };

    struct GridCell: public Drawable {
        Vector2 circle_offset;
        Color stroke_col = WHITE;
        Color fill_col = WHITE;
        float t = 0;
        float alive = 0;

        GridCell() {}
        GridCell(const Rectangle &rect) : Drawable(Vector2{rect.x, rect.y}, Vector2{rect.width, rect.height}) {
            circle_offset = Vector2 { randf(), randf() };
        }

        Rectangle get_rect_padded(float padding) {
            return centered_padded_rectangle(Rectangle {
                position.x, position.y, dimensions.x, dimensions.y
            }, padding);
        }

        Rectangle get_stroke_rect() {
            return get_rect_padded(0);
        }

        Rectangle get_fill_rect() {
            return centered_padded_rectangle(Rectangle {
                position.x, position.y, dimensions.x, dimensions.y
            }, 6);
        }

        Color get_stroke_col() {
            return Color {
                stroke_col.r,
                stroke_col.g,
                stroke_col.b,
                (unsigned char)(255 * t)
            };
        }

        Color get_fill_col() {
            return Color {
                fill_col.r,
                fill_col.g,
                fill_col.b,
                (unsigned char)(255 * alive * t)
            };
        }

        Circle get_occluder() {
            return get_rectangle_occluding_circle_offset(Rectangle {
                position.x, position.y, dimensions.x, dimensions.y
            }, circle_offset, t);
        }
    };


    class String {
    private:
        std::string text;

        int width, height;

    public:
        Font font;
        int font_size;
        float t = 0;

        String() {}
        String(std::string _text) {
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
};