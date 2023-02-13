#include <vector>
#include "stage/scene.hpp"

SceneController* Scene::get_scene_controller() {
    if (!has_scene_controller) {
        std::cout << "THE SCENE DOESN'T HAVE A SCENE CONTROLLER, TERMINATING!\n";
        exit(1);
    }

    return scene_controller;
}
void Scene::set_scene_controller(SceneController* new_scene_controller) {
    scene_controller = new_scene_controller;
    has_scene_controller = true;
}