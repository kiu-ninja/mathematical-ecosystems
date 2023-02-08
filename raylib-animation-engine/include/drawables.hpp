#pragma once
#include <cmath>
#include "raylib.h"
#include "random.hpp"
#include "easing.hpp"
#include "vectors.hpp"
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

        virtual void draw() {};

        StatelessScene* move_to(const Vector2 &destination) {
            return Interpolate::interpolate<Vector2>(
                &this->position, 
                destination,
                Interpolate::Mode::CUBIC, 
                Interpolate::Behavior::STATIC
            );
        }

        virtual StatelessScene* translate(const Vector2 &offset) {
            return Interpolate::interpolate<Vector2>(
                &this->position, 
                offset, 
                Interpolate::Mode::CUBIC, 
                Interpolate::Behavior::RELATIVE_DELTA
            );
        }

        virtual StatelessScene* scale(const Vector2 &factor) {
            return Interpolate::interpolate<Vector2>(
                &this->dimensions, 
                factor, 
                Interpolate::Mode::CUBIC, 
                Interpolate::Behavior::RELATIVE_FACTOR
            );
        }

        virtual StatelessScene* scale(const float &factor) {
            return scale(Vector2 { factor, factor });
        }

        virtual Vector2 closest_point(Vector2 point) {
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
    };

    // 

    template<typename T>
    struct Group: public Drawable {
        std::vector<T*> objects;

        Group() {}

        void add(T* object) {
            objects.push_back(object);
        }

        int size() {
            return objects.size();
        }

        T* operator[](const int &i) {
            if (i >= objects.size()) {
                while (objects.size() < i + 1) {
                    objects.push_back(new T());
                }
            }
            return (T*)objects[i];
        }

        SceneGroup* translate(const Vector2 &offset) override {
            SceneGroup* sg = new SceneGroup();

            for (T* object : objects) {
                sg->merge(object->translate(offset));
            }

            return sg;
        }

        SceneGroup* scale(const float &factor) override {
            return scale(Vector2 { factor, factor });
        }

        SceneGroup* scale(const Vector2 &factor) override {
            SceneGroup* res = space_out(factor);
            for (T* object : objects) {
                res->merge(object->scale(factor));
            }
            return res;
        }

        /// @brief Scales the distance between objects but doesn't scale the objects themselves
        /// @param factor 
        /// @return SceneGroup*
        SceneGroup* space_out(const float &factor) {
            return space_out(Vector2 { factor, factor });
        }

        /// @brief Scales the distance between objects but doesn't scale the objects themselves
        /// @param factor 
        /// @return SceneGroup*
        SceneGroup* space_out(const Vector2 &factor) {
            struct DrawableGroupScaleScene: public SceneGroup {
                using SceneGroup::SceneGroup;

                std::vector<T*> objects;
                Vector2 pivot, factor;

                void start() override {
                    for (T* object : objects) {
                        // this->merge(object->scale(factor));
                        this->merge(object->translate((object->position - pivot) * (factor - Vector2 { 1, 1 })));
                    }
                }

                void update_state(const float &t) override {

                }
            };

            Vector2 center = Vector2 { 0, 0 };
            for (T* object : objects) {
                center += object->position;
            }
            center = center / objects.size();

            DrawableGroupScaleScene* res = new DrawableGroupScaleScene();

            res->objects = this->objects;
            res->pivot = center;
            res->factor = factor;

            return res;
        }

        void draw() override {
            for (T* object : objects) {
                object->draw();
            }
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

        virtual void draw() override {
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
            return get_rect_padded(dimensions.x / 10);
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

        void draw() override {
            if (visibility > 0)
                draw_rectangle_circle_bounded(
                    this->get_stroke_rect(), 
                    this->get_occluder(), 
                    this->get_stroke_col()
                );
            if (alive * visibility > 0)
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
        float alpha = 0;

        int width, height;

        int font_size;
        float t = 1;

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

        Rectangle get_background_rect() {
            Vector2 s = Vector2 { (float)width * font_size / 2, (float)height * font_size };
            Vector2 p = this->position - s / 2;
            return padded_rectangle(Rectangle { p.x, p.y, s.x, s.y }, -font_size / 2);
        }

        Vector2 get_pos() {
            return this->position - Vector2 { (float)width * font_size / 4, (float)height * font_size / 2 };
        }

        StatelessScene* write(const std::string &target_text) {
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

        StatelessScene* appear() {
            return Interpolate::interpolate<float>(&this->alpha, 1, Interpolate::Mode::CUBIC_CUBIC, Interpolate::Behavior::STATIC);
        }

        StatelessScene* disappear() {
            return Interpolate::interpolate<float>(&this->alpha, 0, Interpolate::Mode::CUBIC_CUBIC, Interpolate::Behavior::STATIC);
        }

        void draw() override {
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
    };

    struct CircledString: public String {
        using String::String;

        void draw() override {
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
    };

    struct Arrow: public Drawable {
        Color col = WHITE;
        float weight = 4, head_size = 20;
        float visibility = 0;

        Drawable *_from = this, *_to = this;

        using Drawable::Drawable;

        Arrow* from(Drawable* object) {
            this->_from = object;

            return this;
        }

        Arrow* to(Drawable* object) {
            this->_to = object;

            return this;
        }

        StatelessScene* appear() {
            return Interpolate::interpolate<float>(&this->visibility, 1, Interpolate::Mode::CUBIC_CUBIC, Interpolate::Behavior::STATIC);
        }

        StatelessScene* disappear() {
            return Interpolate::interpolate<float>(&this->visibility, 0, Interpolate::Mode::CUBIC_CUBIC, Interpolate::Behavior::STATIC);
        }

        void draw() override {
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
    };
}; 