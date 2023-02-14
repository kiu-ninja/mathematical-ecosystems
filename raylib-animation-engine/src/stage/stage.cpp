#include <iostream>
#include <vector>
#include "stage/stage.hpp"
#include "stage/scene/instant_builder.hpp"
#include "stage/scene/builders.hpp"

void Stage::scene_update() {
    Scene::Controller* sc_sc = scene_composition->get_scene_controller();
    sc_sc->update();
    if (sc_sc->should_finish()) {
        scene_composition->finish();
    }
    if (sc_sc->should_act()) {
        if (!sc_sc->has_begun()) {
            scene_composition->begin();
        }
        scene_composition->act();
    }

    background_update();
} 

void Stage::add_scene_after_last(Scene::Scene* scene) {
    Scene::Controller* sc = get_last_scene_controller();
    set_last_scene_controller(scene->get_scene_controller());

    add_scene(Scene::Builders::waiting_for(
        scene,
        sc
    ));
} 

void Stage::add_scene(Scene::Scene* scene) {
    set_last_scene_controller(scene->get_scene_controller());

    setting_last_scene_controller = true;

    scene_composition->add(scene);
}

Scene::Controller* Stage::get_last_scene_controller() {
    if (!has_last_scene_controller) {
        std::cout << "THE STAGE DOESN'T HAVE A LAST SCENE CONTROLLER, TERMINATING!\n";
        exit(EXIT_FAILURE);
    }
    
    return last_scene_controller;
}

void Stage::set_last_scene_controller(Scene::Controller* scene_controller) {
    if (setting_last_scene_controller) {
        last_scene_controller = scene_controller;
        has_last_scene_controller = true;
        setting_last_scene_controller = false;
    }
}
