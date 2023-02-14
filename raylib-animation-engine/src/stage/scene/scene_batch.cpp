#include <vector>
#include "stage/scene/batch.hpp"

namespace Scene {
    void Batch::set_scene_controller(Controller* new_scene_controller) {
        for (Scene* &s : scenes) {
            s->set_scene_controller(new_scene_controller);
        }
        scene_controller = new_scene_controller;
        has_scene_controller = true;
    }

    Scene* Batch::begin() {
        for (Scene* &s : scenes) {
            s = s->begin();
        }
        return this;
    }

    Scene* Batch::finish() {
        for (Scene* &s : scenes) {
            s = s->finish();
        }
        return this;
    }

    void Batch::act() {
        for (Scene* s : scenes) {
            s->act();
        }
    }

    void Batch::add(Scene* scene) {
        if (has_scene_controller)
            scene->set_scene_controller(get_scene_controller());
        scenes.push_back(scene);
    }
}