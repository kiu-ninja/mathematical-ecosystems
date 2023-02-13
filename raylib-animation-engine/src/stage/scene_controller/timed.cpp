#include <iostream>
#include "stage/scene_controller/timed.hpp"

TimedSceneController::TimedSceneController(float start_time_seconds, float duration_seconds) {
    at_seconds(start_time_seconds);
    for_seconds(duration_seconds);
    current_frame = -1;
}
TimedSceneController::TimedSceneController(int start_time_frames, int duration_frames) {
    at_frames(start_time_frames);
    for_frames(duration_frames);
    current_frame = -1;
}

void TimedSceneController::at_seconds(float time_seconds) {
    at_frames(time_seconds * 60);
}
void TimedSceneController::at_frames(int time_frames) {
    start_time = time_frames;
}
void TimedSceneController::for_seconds(float time_seconds) {
    for_frames(time_seconds * 60);
}
void TimedSceneController::for_frames(int time_frames) {
    duration = time_frames;
}
void TimedSceneController::in_seconds(float time_seconds) {
    in_frames(time_seconds * 60);
}
void TimedSceneController::in_frames(int time_frames) {
    start_time += time_frames;
}

void TimedSceneController::update() {
    if (should_act() && !should_finish()) {
        current_frame++;
    }

    if (!should_act()) {
        start_time--;
    }
};
bool TimedSceneController::should_act() {
    return this->start_time <= 0;
};
bool TimedSceneController::should_finish() {
    return this->current_frame + 1 >= this->duration;
}
float TimedSceneController::get_t() {
    return (float)this->current_frame / this->duration;
}