#include "stage/scene_controller/infinite.hpp"

void InfiniteSceneController::update() {
    current_frame = 0;
}
bool InfiniteSceneController::should_act() {
    return true;
}
bool InfiniteSceneController::should_finish() {
    return false;
}
float InfiniteSceneController::get_t() {
    return 1.0f;
}