#include <vector>
#include "stage/batch_scene.hpp"

void BatchScene::act() {
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

// 

Scene* BatchScene::add(Scene* scene) {
    scenes.push_back(scene);
    number_of_scenes++;

    return scene;
}