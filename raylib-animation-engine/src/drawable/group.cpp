#include "drawables.hpp"
#include "drawing_functions.hpp"
#include "stage/scene.hpp"
#include "stage/scene.hpp"
#include "stage/scene.hpp"

using namespace Drawables;

void Group::add(Drawable* object) {
    objects.push_back(object);
}

int Group::size() {
    return objects.size();
}

Scene::Builders::Instant* Group::translate(const Vector2 &offset) {
    return new Scene::Builders::Instant([=] () {
        Scene::Batch* sb = new Scene::Batch();
        
        for (Drawable* object : objects) {
            sb->add(object->translate(offset));
        }

        return (Scene::Scene*)sb;
    });
}

Scene::Builders::Instant* Group::scale(const float &factor, const Vector2 &pivot) {
    return scale(Vector2 { factor, factor }, pivot);
}

Scene::Builders::Instant* Group::scale(const Vector2 &factor, const Vector2 &pivot) {
    return new Scene::Builders::Instant([=] () {
        Scene::Batch* sb = new Scene::Batch();
        
        for (Drawable* object : objects) {
            sb->add(object->translate((object->position - pivot) * (factor - Vector2 { 1, 1 })));
            sb->add(object->scale(factor));
        }

        return (Scene::Scene*)sb;
    });
}

Scene::Builders::Instant* Group::scale(const float &factor) {
    return scale(Vector2 { factor, factor });
}

Scene::Builders::Instant* Group::scale(const Vector2 &factor) {
    Vector2 min = objects[0]->position, max = objects[0]->position;
    for (Drawable* object : objects) {
        min.x = std::min(object->position.x, min.x);
        min.y = std::min(object->position.y, min.y);
        max.x = std::max(object->position.x, max.x);
        max.y = std::max(object->position.y, max.y);
    }
    Vector2 center = (min + max) / 2;

    return scale(factor, center);
}

Scene::Builders::Instant* Group::space_out(const float &factor) {
    return space_out(Vector2 { factor, factor });
}

Scene::Builders::Instant* Group::space_out(const Vector2 &factor) {
    Vector2 min = objects[0]->position, max = objects[0]->position;
    for (Drawable* object : objects) {
        min.x = std::min(object->position.x, min.x);
        min.y = std::min(object->position.y, min.y);
        max.x = std::max(object->position.x, max.x);
        max.y = std::max(object->position.y, max.y);
    }
    Vector2 center = (min + max) / 2;

    return new Scene::Builders::Instant([=] () {
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