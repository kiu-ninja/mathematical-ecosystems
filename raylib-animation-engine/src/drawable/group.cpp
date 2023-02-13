#include "drawables.hpp"
#include "drawing_functions.hpp"
#include "stage/scene_builder.hpp"
#include "stage/scene_batch.hpp"
#include "stage/scene_controller/timed.hpp"

using namespace Drawables;

void Group::add(Drawable* object) {
    objects.push_back(object);
}

int Group::size() {
    return objects.size();
}

SceneBuilder* Group::translate(const Vector2 &offset) {
    return new SceneBuilder([=] () {
        SceneBatch* sb = new SceneBatch();
        
        for (Drawable* object : objects) {
            sb->add(object->translate(offset));
        }

        return sb;
    }, new InfiniteSceneController());
}

SceneBuilder* Group::scale(const float &factor) {
    return scale(Vector2 { factor, factor });
}

SceneBuilder* Group::scale(const Vector2 &factor) {
    Vector2 center = Vector2 { 0, 0 };
    for (Drawable* object : objects) {
        center += object->position;
    }
    center = center / objects.size();

    return new SceneBuilder([=] () {
        SceneBatch* sb = new SceneBatch();
        
        for (Drawable* object : objects) {
            sb->add(object->translate((object->position - center) * (factor - Vector2 { 1, 1 })));
            sb->add(object->scale(factor));
        }

        return sb;
    }, new InfiniteSceneController());
}

SceneBuilder* Group::space_out(const float &factor) {
    return space_out(Vector2 { factor, factor });
}

SceneBuilder* Group::space_out(const Vector2 &factor) {
    Vector2 center = Vector2 { 0, 0 };
    for (Drawable* object : objects) {
        center += object->position;
    }
    center = center / objects.size();

    return new SceneBuilder([=] () {
        SceneBatch* sb = new SceneBatch();
        
        for (Drawable* object : objects) {
            sb->add(object->translate((object->position - center) * (factor - Vector2 { 1, 1 })));
        }

        return sb;
    }, new InfiniteSceneController());
}

void Group::draw() {
    for (Drawable* object : objects) {
        object->draw();
    }
}