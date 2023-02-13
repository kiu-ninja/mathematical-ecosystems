#pragma once
#include <iostream>
#include <vector>
#include "stage/scene.hpp"
#include "stage/scene_composition.hpp"
#include "stage/scene_controller/infinite.hpp"

// ================

class Stage {
public:
    SceneComposition* scene_composition;

public:
    Stage() {
        scene_composition = new SceneComposition();
        scene_composition->set_scene_controller(new InfiniteSceneController());
    }
    Scene* add_scene_after_last(Scene* scene);
    Scene* add_scene(Scene* scene);
    Scene* last_scene();

    void scene_update();
    virtual void background_update() = 0;
};