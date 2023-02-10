#include <iostream>
#include <vector>
// #include "application/application->hpp"
// #include "application/application_with_state.hpp"
#include "stage/scene.hpp"
#include "stage/stage.hpp"
// #include "stage/scene_group.hpp"
// #include "stage/scene_builder.hpp"

void Stage::scene_update() {
    scene_composition->update();
    if (scene_composition->should_act()) {
        if (!scene_composition->has_begun()) {
            scene_composition->begin();
        }
        scene_composition->act();
    }
    if (scene_composition->should_finish()) {
        scene_composition->finish();

        // CloseWindow();
        // exit(0);
    }

    background_update();
}

Scene* Stage::add_scene_after_last(Scene* scene) {
    return scene_composition->add_after_last(scene);
}

Scene* Stage::add_scene(Scene* scene) {
    return scene_composition->add(scene);
}

Scene* Stage::last_scene() {
    return scene_composition->last_scene();
}