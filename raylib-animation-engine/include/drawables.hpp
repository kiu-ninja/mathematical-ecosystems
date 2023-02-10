#pragma once
#include <cmath>
#include "raylib.h"
#include "random.hpp"
#include "easing.hpp"
#include "vectors.hpp"
#include "stage/.hpp"
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

        /* Animates the drawable moving to a given destination. */
        TimedScene* move_to(const Vector2 &destination);
        /* Animates the drawable translating with a given offset. */
        virtual TimedScene* translate(const Vector2 &offset);
        /* Animates the drawable scaling with a given vector.

        The drawable will get scaled by factor.x in the x dimension and factor.y in the y dimension. */
        virtual TimedScene* scale(const Vector2 &factor);
        /* Animates the drawable scaling with a given factor. */
        virtual TimedScene* scale(const float &factor);
        /* Returns the closest point to to the drawable from the given position. */
        virtual Vector2 closest_point(Vector2 point);
        /* Draws the drawable to the window. */
        virtual void draw() {};
    };

    // 

    struct Group: public Drawable {
        std::vector<Drawable*> objects;

        Group() {}

        /* Adds a drawable to the group. */
        void add(Drawable* object);
        /* Returns the size of the group. */
        int size();
        /* Gets the i-th object in the group.
        
        If the size of the group is smaller than the index requested, it will get filled with new Drawables. */
        template<typename T> T* get(const int &i) {
            while (objects.size() < i + 1) {
                objects.push_back(new T());
            }

            return (T*)objects[i];
        };
        /* Animates the group translating with a given offset. */
        BatchScene* translate(const Vector2 &offset) override;
        /* Animates the drawable scaling with a given factor. */
        BatchScene* scale(const float &factor) override;
        /* Animates the group scaling with a given vector.

        The group will get scaled by factor.x in the x dimension and factor.y in the y dimension. */
        BatchScene* scale(const Vector2 &factor) override;
        /* Animates the distance between group objects scaling with a given factor. */
        BatchScene* space_out(const float &factor);
        /* Animates the distance between group objects scaling with a given vector.

        The distance btween group objects will get scaled by factor.x in the x dimension and factor.y in the y dimension. */
        BatchScene* space_out(const Vector2 &factor);
        /* Draws the drawable to the window. */
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

        /* Returns the rectangle to be drawn. */
        Rectangle get_rect();
        /* Returns the padded rectangle to be drawn. */
        Rectangle get_rect_padded(float padding);
        /* Draws the `Rect` to the window. */
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
        Scene* animate_alive(const float &new_alive);
        Scene* animate_visibility(const float &new_visibility);
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
        Scene* write(const std::string &target_text);
        Scene* appear();
        Scene* disappear();
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
        Scene* appear();
        Scene* disappear();
        void draw() override;
    };
}; 