#include <vector>
#include "stage/scene_composition.hpp"

void SceneComposition::act() {
    for (Scene* s : scenes) {
        s->update();
    }

    for (int i = 0; i < scenes.size(); i++) {
        if (scenes[i]->should_act()) {
            if (!scenes[i]->has_begun()) {
                scenes[i] = scenes[i]->begin();
            }
            scenes[i]->act();
        }
        if (scenes[i]->should_finish()) {
            for (Scene* scene : scenes[i]->finish()) {
                add(scene);
            }
            scenes.erase(scenes.begin() + i);
            i--;
        }
    }
    current_frame++;
}

bool SceneComposition::should_finish() {
    bool res = true;

    for (Scene* s : scenes) {
        res &= s->should_finish();
    }

    return res;
}

// 

Scene* SceneComposition::add(Scene* scene) {
    scenes.push_back(scene);
    number_of_scenes++;

    return scene;
}
Scene* SceneComposition::add_with_last(Scene* scene) {
    return add(scene->play_with(last_scene()));
}
Scene* SceneComposition::add_after_last(Scene* scene) {
    return add(scene->play_after(last_scene()));
}
Scene* SceneComposition::last_scene() {
    return scenes[number_of_scenes - 1];
}