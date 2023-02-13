#include "stage/scene_controller/composition.hpp"

SceneCompositionController::SceneCompositionController(std::vector<Scene*> &sc) : handle(sc) {
    // handle = sc;
}
void SceneCompositionController::update() {
    current_frame = 0;
}
bool SceneCompositionController::should_act() {
    return true;
}
bool SceneCompositionController::should_finish() {
    for (Scene* scene : handle) {
        if (!scene->get_scene_controller()->should_finish()) {
            return false;
        }
    }
    return true;
}
float SceneCompositionController::get_t() {
    return 1.0f;
}