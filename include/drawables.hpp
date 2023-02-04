#pragma once
#include <cmath>
#include "raylib.h"
#include "random.hpp"
#include "easing.hpp"
#include "vectors.h"
#include "stage.hpp"
#include "circle.hpp"
#include "drawing_functions.hpp"
#include "single_field_interpolation.hpp"

namespace Drawable {
    Rectangle padded_rectangle(const Rectangle &rect, const float &padding) {
        return Rectangle {rect.x + padding, rect.y + padding, rect.width - padding * 2, rect.height - padding * 2};
    }

    Rectangle centered_rectangle(const Rectangle &rect) {
        return Rectangle { rect.x - rect.width / 2, rect.y - rect.height / 2, rect.width, rect.height };
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

    
    //


    struct Drawable {
        Vector2 position;
        Vector2 dimensions;

        Drawable() {};
        Drawable(Vector2 _position) : position(_position) {}
        Drawable(Vector2 _position, Vector2 _dimensions) : position(_position), dimensions(_dimensions) {};

        void draw() {};

        template<typename Data>
        struct DrawableTransformScene: public StatelessScene { using StatelessScene::StatelessScene;
            Drawable* object;
            Data initial, target;

            DrawableTransformScene* set_object(Drawable* _object) {
                object = _object;
                return this;
            }

            DrawableTransformScene* set_target(const Data _target) {
                target = _target;
                return this;
            }
        };

        StatelessScene* move_to(const Vector2 &destination) {
            return Interpolate::interpolate<Vector2>(
                &this->position, 
                destination,
                Interpolate::Mode::CUBIC, 
                Interpolate::Behavior::STATIC
            );
        }

        StatelessScene* translate(const Vector2 &offset) {
            return Interpolate::interpolate<Vector2>(
                &this->position, 
                offset, 
                Interpolate::Mode::CUBIC, 
                Interpolate::Behavior::RELATIVE_DELTA
            );
        }

        StatelessScene* scale(const Vector2 &factor) {
            return Interpolate::interpolate<Vector2>(
                &this->dimensions, 
                factor, 
                Interpolate::Mode::CUBIC, 
                Interpolate::Behavior::RELATIVE_FACTOR
            );
        }
    };


    // 


    template<typename T>
    struct Group: public Drawable {
        std::vector<T*> objects;

        Group() {}

        void add(T* object) {
            objects.push_back(object);
        }

        T* operator[](const int &i) {
            return objects[i];
        }

        SceneGroup* translate(const Vector2 &offset) {
            SceneGroup* scenes = new SceneGroup();

            for (T* object : objects) {
                scenes->add(object->translate(offset));
            }

            return scenes;
        }

        SceneGroup* scale(const Vector2 &factor) {
            Vector2 center = Vector2 { 0, 0 };
            for (T* object : objects) {
                center += object->position;
            }
            center = center / objects.size();

            SceneGroup* scenes = new SceneGroup();

            for (T* object : objects) {
                scenes->add(object->scale(factor));
                scenes->add(object->translate(Vector2 {
                    (center.x - object->position.x) * factor.x,
                    (center.y - object->position.y) * factor.y
                }));
            }

            return scenes;
        }
    };


    //


    struct Rect: public Drawable {
        Vector2 circle_offset;
        float visibility = 0;

        Rect() {}
        Rect(const Rectangle &rect) : Drawable(Vector2{rect.x, rect.y}, Vector2{rect.width, rect.height}) {
            circle_offset = Vector2 { randf(), randf() };
        }

        Rectangle get_rect() {
            return get_rect_padded(0);
        }

        Rectangle get_rect_padded(float padding) {
            return padded_rectangle(centered_rectangle(Rectangle {
                position.x, position.y, dimensions.x, dimensions.y
            }), padding);
        }

        Circle get_occluder() {
            return get_rectangle_occluding_circle_offset(Rectangle {
                position.x, position.y, dimensions.x, dimensions.y
            }, circle_offset, visibility);
        }

        void draw() {
            draw_rectangle_circle_bounded(
                this->get_rect(),
                this->get_occluder(), 
                WHITE
            );
        }
    };

    
    //


    struct GridCell: public Rect {
        Color stroke_col = WHITE;
        Color fill_col = WHITE;
        float alive = 0;

        using Rect::Rect;

        Rectangle get_stroke_rect() {
            return get_rect_padded(0);
        }

        Rectangle get_fill_rect() {
            return get_rect_padded(6);
        }

        Color get_stroke_col() {
            return Color {
                stroke_col.r,
                stroke_col.g,
                stroke_col.b,
                (unsigned char)(255 * visibility)
            };
        }

        Color get_fill_col() {
            return Color {
                fill_col.r,
                fill_col.g,
                fill_col.b,
                (unsigned char)(255 * alive * visibility)
            };
        }

        StatelessScene* animate_alive(const float &new_alive) {
            return Interpolate::interpolate<float>(
                &this->alive, 
                new_alive, 
                Interpolate::Mode::CUBIC, 
                Interpolate::Behavior::STATIC
            );
        }

        StatelessScene* animate_visibility(const float &new_visibility) {
            return Interpolate::interpolate<float>(
                &this->visibility, 
                new_visibility, 
                Interpolate::Mode::CUBIC, 
                Interpolate::Behavior::STATIC
            );
        }

        void draw() {
            draw_rectangle_circle_bounded(
                this->get_stroke_rect(), 
                this->get_occluder(), 
                this->get_stroke_col()
            );
            DrawRectangleRec(
                this->get_fill_rect(), 
                this->get_fill_col()
            );
        }
    };


    //


    struct String: public Drawable {
        std::string text;
        Font font;
        Color col = WHITE;
        float alpha = 1;

        int width, height;

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

        Vector2 get_pos() {
            return this->position - Vector2 { (float)width * font_size / 4, (float)height * font_size / 2 };
        }

        StatelessScene* write(const std::string &target_text) {
            struct AnimateAlive: public DrawableTransformScene<std::string> { using DrawableTransformScene<std::string>::DrawableTransformScene;
                void start() {
                    ((String *)this->object)->update_text(this->target);
                    ((String *)this->object)->t = 0;
                }

                void update_state(const float &t) {
                    ((String *)this->object)->t = Easing::cubic(0, 1, t);
                }
            };

            AnimateAlive* res = new AnimateAlive(1.0f);
            res->set_target(target_text);
            res->set_object(this);
            ((StatelessScene*)res)->set_duration(target_text.size() / 10.0f);

            return res;
        }

        StatelessScene* appear() {
            return Interpolate::interpolate<float>(&this->alpha, 1, Interpolate::Mode::CUBIC_CUBIC, Interpolate::Behavior::STATIC);
        }

        StatelessScene* disappear() {
            return Interpolate::interpolate<float>(&this->alpha, 0, Interpolate::Mode::CUBIC_CUBIC, Interpolate::Behavior::STATIC);
        }

        void draw() {
            DrawTextEx(
                this->font, 
                this->get_string().c_str(), 
                this->get_pos(), 
                this->font_size, 
                0, 
                Color{ col.r, col.g, col.b, (unsigned char)(clamp(alpha * 255, 0.0f, 255.0f)) }
            );
        }
    };
}; 