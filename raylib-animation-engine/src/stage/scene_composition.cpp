#include <vector>
#include "stage/scene_controller/composition.hpp"
#include "stage/scene_composition.hpp"

SceneComposition::SceneComposition() {
    set_scene_controller(new SceneCompositionController(scenes));
}

Scene* SceneComposition::begin() {
    set_scene_controller(new SceneCompositionController(scenes));
    return this;
}


Scene* SceneComposition::finish() {
    return this;
}


void SceneComposition::act() {
    for (int i = 0; i < scenes.size(); i++) {
        if (scenes[i]->get_scene_controller()->should_finish()) {
            Scene* scene = scenes[i]->finish();
            if (scene != scenes[i]) {
                scenes[i] = scene;
            } else {
                scenes.erase(scenes.begin() + i);
                i--;
            }
            continue;
        }
        Scene* pscene;
        while (scenes[i]->get_scene_controller()->should_act() && !scenes[i]->get_scene_controller()->has_begun() && pscene != scenes[i]) {
            pscene = scenes[i];
            scenes[i] = scenes[i]->begin();
        }
        if (scenes[i]->get_scene_controller()->should_act()) {
            scenes[i]->act();
        }
    }
                                                    
    for (Scene* s : scenes) {
        s->get_scene_controller()->update();
    }
}


Scene* SceneComposition::add(Scene* scene) {
    scenes.push_back(scene);
    number_of_scenes++;

    return scene;
}


Scene* SceneComposition::last_scene() {
    if (number_of_scenes <= 0)
        return nullptr;

    return scenes[number_of_scenes - 1];
}