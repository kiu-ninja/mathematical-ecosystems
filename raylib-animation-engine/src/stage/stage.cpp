#include <iostream>
#include <vector>
// #include "application/application->hpp"
// #include "application/application_with_state.hpp"
#include "stage/scene.hpp"
#include "stage/stage.hpp"
// #include "stage/scene_group.hpp"
// #include "stage/scene_builder.hpp"

void Stage::scene_update() {
    SceneController* sc_sc = scene_composition->get_scene_controller();
    sc_sc->update();
    if (sc_sc->should_finish()) {
        scene_composition->finish();

        /* NOTE: If you want to close the window after the animations finish, do this:
        CloseWindow();
        exit(0);
        */
    }
    if (sc_sc->should_act()) {
        if (!sc_sc->has_begun()) {
            scene_composition->begin();
        }
        scene_composition->act();
    }

    background_update();
}

Scene* Stage::add_scene_after_last(Scene* scene) {
    // FIXME: what the hell is going on here
    // return scene_composition->add_after_last(scene);
    return nullptr;
}

Scene* Stage::add_scene(Scene* scene) {
    return scene_composition->add(scene);
}

Scene* Stage::last_scene() {
    return scene_composition->last_scene();
}