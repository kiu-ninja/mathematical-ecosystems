#include "drawables.hpp"
#include "drawing_functions.hpp"
#include "stage/batch_scene.hpp"

using namespace Drawables;

void Group::add(Drawable* object) {
    objects.push_back(object);
}

int Group::size() {
    return objects.size();
}

BatchScene* Group::translate(const Vector2 &offset) {
    BatchScene* sg = new BatchScene();

    for (Drawable* object : objects) {
        sg->add(object->translate(offset));
    }

    return sg;
}

BatchScene* Group::scale(const float &factor) {
    return scale(Vector2 { factor, factor });
}

BatchScene* Group::scale(const Vector2 &factor) {
    BatchScene* res = space_out(factor);
    for (Drawable* object : objects) {
        res->add(object->scale(factor));
    }
    return res;
}

BatchScene* Group::space_out(const float &factor) {
    return space_out(Vector2 { factor, factor });
}

BatchScene* Group::space_out(const Vector2 &factor) {
    class DrawableGroupScaleScene: public BatchScene, public TimedScene {
    public:
        using TimedScene::TimedScene;

        std::vector<Drawable*> objects;
        Vector2 pivot, factor;

        Scene* begin() override {
            for (Drawable* object : objects) {
                this->add(object->translate((object->position - pivot) * (factor - Vector2 { 1, 1 }))->duration_seconds(2));
            }

            std::cout << "AAAAAA\n";
            current_frame = 0;

            this->add(new DebugScene("Scene is happening"));

            return this;
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
    res->duration_seconds(1);

    // class TimedBatchScene: public BatchScene, public TimedScene {};

    return res;
}

void Group::draw() {
    for (Drawable* object : objects) {
        object->draw();
    }
}