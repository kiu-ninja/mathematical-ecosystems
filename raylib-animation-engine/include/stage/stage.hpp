#pragma once
#include <iostream>
#include <vector>
#include "stage/scene.hpp"
#include "stage/scene_composition.hpp"

// ================

class Stage {
public:
    SceneComposition* scene_composition;

public:
    Stage() {
        scene_composition = new SceneComposition();
    }
    Scene* add_scene_after_last(Scene* scene);
    // SceneGroup* merge_scene_with_last(Scene* scene);
    // SceneGroup* add_scene_to_last(Scene* scene);
    // SceneBuilder* add_scene_builder(std::function<void(SceneGroup*)> f);
    Scene* add_scene(Scene* scene);
    // SceneGroup* new_scene_group();
    Scene* last_scene();

    void scene_update();
    virtual void background_update() = 0;
};