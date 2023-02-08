#include "drawables.hpp"
#include "drawing_functions.hpp"

using namespace Drawables;

void Group::add(Drawable* object) {
    objects.push_back(object);
}

int Group::size() {
    return objects.size();
}

SceneGroup* Group::translate(const Vector2 &offset) {
    SceneGroup* sg = new SceneGroup();

    for (Drawable* object : objects) {
        sg->merge(object->translate(offset));
    }

    return sg;
}

SceneGroup* Group::scale(const float &factor) {
    return scale(Vector2 { factor, factor });
}

SceneGroup* Group::scale(const Vector2 &factor) {
    SceneGroup* res = space_out(factor);
    for (Drawable* object : objects) {
        res->merge(object->scale(factor));
    }
    return res;
}

/// @brief Scales the distance between objects but doesn't scale the objects themselves
/// @param factor 
/// @return SceneGroup*
SceneGroup* Group::space_out(const float &factor) {
    return space_out(Vector2 { factor, factor });
}

/// @brief Scales the distance between objects but doesn't scale the objects themselves
/// @param factor 
/// @return SceneGroup*
SceneGroup* Group::space_out(const Vector2 &factor) {
    struct DrawableGroupScaleScene: public SceneGroup {
        using SceneGroup::SceneGroup;

        std::vector<Drawable*> objects;
        Vector2 pivot, factor;

        void start() override {
            for (Drawable* object : objects) {
                // this->merge(object->scale(factor));
                this->merge(object->translate((object->position - pivot) * (factor - Vector2 { 1, 1 })));
            }
        }

        void update_state(const float &t) override {

        }
    };

    Vector2 center = Vector2 { 0, 0 };
    for (Drawable* object : objects) {
        center += object->position;
    }
    center = center / objects.size();

    DrawableGroupScaleScene* res = new DrawableGroupScaleScene();

    res->objects = this->objects;
    res->pivot = center;
    res->factor = factor;

    return res;
}

void Group::draw() {
    for (Drawable* object : objects) {
        object->draw();
    }
}