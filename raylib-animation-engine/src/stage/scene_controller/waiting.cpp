#include "stage/scene_controller/waiting.hpp"

WaitingSceneController::WaitingSceneController(SceneController* waiting_for) {
    this->handle = waiting_for;
    current_frame = -1;
}

void WaitingSceneController::update() {
    if (should_act()) {
        // current_frame = 0;
    }
}
bool WaitingSceneController::should_act() {
    return this->handle->should_finish();
}
bool WaitingSceneController::should_finish() {
    return false;
}
float WaitingSceneController::get_t() {
    return 1.0f;
}