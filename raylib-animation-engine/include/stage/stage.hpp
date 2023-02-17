#pragma once
#include "stage/scene.hpp"
#include <iostream>
#include <vector>

// ================

class Stage {
private:
    Scene::Composition* scene_composition;

    Scene::Controller* last_scene_controller;
    bool has_last_scene_controller = false;
    bool setting_last_scene_controller = true;
    void set_last_scene_controller(Scene::Controller* scene_controller);

public:
    Stage()
    {
        scene_composition = new Scene::Composition();
        scene_composition->set_scene_controller(new Scene::Controllers::Infinite());
    }
    void add_scene_after_last(Scene::Scene* scene);
    void add_scene(Scene::Scene* scene);
    Scene::Controller* get_last_scene_controller();

    void scene_update();
    virtual void background_update() = 0;
};