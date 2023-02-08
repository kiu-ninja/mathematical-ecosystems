#pragma once
#include <cmath>
#include "raylib.h"
#include "random.hpp"
#include "easing.hpp"
#include "vectors.hpp"
#include "stage.hpp"
#include "circle.hpp"
#include "single_field_interpolation.hpp"

namespace Drawables {
    Rectangle padded_rectangle(const Rectangle &rect, const float &padding);
    Rectangle centered_rectangle(const Rectangle &rect);
    Circle get_rectangle_occluding_circle_offset(const Rectangle &rect, const Vector2 &circle_offset, const float &t);

    struct Drawable {
        Vector2 position;
        Vector2 dimensions;

        Drawable() {};
        Drawable(Vector2 _position) : position(_position) {}
        Drawable(Vector2 _position, Vector2 _dimensions) : position(_position), dimensions(_dimensions) {};

        virtual void draw() {};

        StatelessScene* move_to(const Vector2 &destination);
        virtual StatelessScene* translate(const Vector2 &offset);
        virtual StatelessScene* scale(const Vector2 &factor);
        virtual StatelessScene* scale(const float &factor);
        virtual Vector2 closest_point(Vector2 point);
    };

    // 

    struct Group: public Drawable {
        std::vector<Drawable*> objects;

        Group() {}

        void add(Drawable* object);
        int size();
        template<typename T>
        T* get(const int &i) {
            while (objects.size() < i + 1) {
                objects.push_back(new T());
            }

            return (T*)objects[i];
        };

        SceneGroup* translate(const Vector2 &offset) override;
        SceneGroup* scale(const float &factor) override;
        SceneGroup* scale(const Vector2 &factor) override;
        /// @brief Scales the distance between objects but doesn't scale the objects themselves
        /// @param factor 
        /// @return SceneGroup*
        SceneGroup* space_out(const float &factor);

        /// @brief Scales the distance between objects but doesn't scale the objects themselves
        /// @param factor 
        /// @return SceneGroup*
        SceneGroup* space_out(const Vector2 &factor);

        void draw() override;
    };

    //

    struct Rect: public Drawable {
        Vector2 circle_offset;
        float visibility = 0;

        Rect() {}
        Rect(const Rectangle &rect) : Drawable(Vector2{rect.x, rect.y}, Vector2{rect.width, rect.height}) {
            circle_offset = Vector2 { randf(), randf() };
        }

        Rectangle get_rect();
        Rectangle get_rect_padded(float padding);
        Circle get_occluder();
        virtual void draw() override;
    };
    
    //

    struct GridCell: public Rect {
        Color stroke_col = WHITE;
        Color fill_col = WHITE;
        float alive = 0;

        using Rect::Rect;

        Rectangle get_stroke_rect();
        Rectangle get_fill_rect();
        Color get_stroke_col();
        Color get_fill_col();
        StatelessScene* animate_alive(const float &new_alive);
        StatelessScene* animate_visibility(const float &new_visibility);
        void draw() override;
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

        std::string get_string();
        void update_text(std::string new_text);
        Rectangle get_background_rect();
        Vector2 get_pos();
        StatelessScene* write(const std::string &target_text);
        StatelessScene* appear();
        StatelessScene* disappear();
        void draw() override;
    };

    struct CircledString: public String {
        using String::String;

        void draw() override;
    };

    struct Arrow: public Drawable {
        Color col = WHITE;
        float weight = 4, head_size = 20;
        float visibility = 0;

        Drawable *_from = this, *_to = this;

        using Drawable::Drawable;

        Arrow* from(Drawable* object);
        Arrow* to(Drawable* object);
        StatelessScene* appear();
        StatelessScene* disappear();
        void draw() override;
    };
}; 