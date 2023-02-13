#include <vector>
#include "stage/scene_batch.hpp"

void SceneBatch::set_scene_controller(SceneController* new_scene_controller) {
    for (Scene* &s : scenes) {
        s->set_scene_controller(new_scene_controller);
    }
    scene_controller = new_scene_controller;
    has_scene_controller = true;
}

Scene* SceneBatch::begin() {
    for (Scene* &s : scenes) {
        s = s->begin();
    }
    return this;
}

Scene* SceneBatch::finish() {
    for (Scene* &s : scenes) {
        s = s->finish();
    }
    return this;
}

void SceneBatch::act() {
    for (Scene* s : scenes) {
        s->act();
    }
}

void SceneBatch::add(Scene* scene) {
    if (has_scene_controller)
        scene->set_scene_controller(get_scene_controller());
    scenes.push_back(scene);
}