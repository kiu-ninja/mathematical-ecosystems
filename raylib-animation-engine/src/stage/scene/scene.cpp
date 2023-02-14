#include <vector>
#include "stage/scene/scene.hpp"

namespace Scene {

    Controller* Scene::get_scene_controller() {
        if (!has_scene_controller) {
            std::cout << "THE SCENE DOESN'T HAVE A SCENE CONTROLLER, TERMINATING!\n";
            exit(EXIT_FAILURE);
        }

        return scene_controller;
    }
    void Scene::set_scene_controller(Controller* new_scene_controller) {
        scene_controller = new_scene_controller;
        has_scene_controller = true;
    }
}