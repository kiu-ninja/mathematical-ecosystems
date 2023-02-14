#include "drawables.hpp"
#include "drawing_functions.hpp"
#include "stage/scene/instant_builder.hpp"
#include "stage/scene/batch.hpp"
#include "stage/scene/controller/timed.hpp"

using namespace Drawables;

void Group::add(Drawable* object) {
    objects.push_back(object);
}

int Group::size() {
    return objects.size();
}

Scene::InstantBuilder* Group::translate(const Vector2 &offset) {
    return new Scene::InstantBuilder([=] () {
        Scene::Batch* sb = new Scene::Batch();
        
        for (Drawable* object : objects) {
            sb->add(object->translate(offset));
        }

        return (Scene::Scene*)sb;
    });
}

Scene::InstantBuilder* Group::scale(const float &factor) {
    return scale(Vector2 { factor, factor });
}

Scene::InstantBuilder* Group::scale(const Vector2 &factor) {
    Vector2 center = Vector2 { 0, 0 };
    for (Drawable* object : objects) {
        center += object->position;
    }
    center = center / objects.size();

    return new Scene::InstantBuilder([=] () {
        Scene::Batch* sb = new Scene::Batch();
        
        for (Drawable* object : objects) {
            sb->add(object->translate((object->position - center) * (factor - Vector2 { 1, 1 })));
            sb->add(object->scale(factor));
        }

        return (Scene::Scene*)sb;
    });
}

Scene::InstantBuilder* Group::space_out(const float &factor) {
    return space_out(Vector2 { factor, factor });
}

Scene::InstantBuilder* Group::space_out(const Vector2 &factor) {
    Vector2 center = Vector2 { 0, 0 };
    for (Drawable* object : objects) {
        center += object->position;
    }
    center = center / objects.size();

    return new Scene::InstantBuilder([=] () {
        Scene::Batch* sb = new Scene::Batch();
        
        for (Drawable* object : objects) {
            sb->add(object->translate((object->position - center) * (factor - Vector2 { 1, 1 })));
        }

        return (Scene::Scene*)sb;
    });
}

void Group::draw() {
    for (Drawable* object : objects) {
        object->draw();
    }
}