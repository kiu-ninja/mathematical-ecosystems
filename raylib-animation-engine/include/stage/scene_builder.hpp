#pragma once

#include "stage/scene.hpp"
#include "stage/scene_composition.hpp"
#include "stage/scene_controller/infinite.hpp"

class SceneBuilder: public Scene {
private:
    std::function<Scene*()> builder;
    SceneController* built_scene_controller;
    bool has_built_scene_controller = false;
    
public:
    SceneBuilder(std::function<Scene*()> f, SceneController* new_built_scene_controller) {
        builder = f;
        set_scene_controller(new InfiniteSceneController());
        set_built_scene_controller(new_built_scene_controller);
    }

    void set_built_scene_controller(SceneController* new_built_scene_controller) {
        built_scene_controller = new_built_scene_controller;
        has_built_scene_controller = true;
    }

    SceneController* get_built_scene_controller() {
        if (!has_built_scene_controller) {
            std::cout << "THE SCENE BUILDER DOESN'T HAVE A BUILT SCENE CONTROLLER, TERMINATING!\n";
            exit(1);
        }

        return built_scene_controller;
    }

    Scene* finish() override {
        return this;
    }

    void act() override {}

    Scene* begin() override {
        Scene* res = builder();
        res->set_scene_controller(get_built_scene_controller());
        return res;
    }
};