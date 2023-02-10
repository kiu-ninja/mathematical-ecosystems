#include <vector>
#include "stage/scene.hpp"

int Scene::get_start_time() {
    return start_time;
}

void Scene::update() {
    if (!should_act()) {
        switch (anchor_type) {
            case PLAYING_AFTER:
                is_start_counting |= anchor->should_finish();
                break;

            case PLAYING_WITH:
                is_start_counting |= anchor->should_act();
                break;
            
            default:
                is_start_counting = true;
                break;
        }

        if (is_start_counting) start_time--;
    }
    if (!should_finish()) {
        current_frame++;
    }
}

Scene* Scene::begin() { 
    current_frame = 0;
    return this; 
};
bool Scene::has_begun() {
    return current_frame >= 0;
}

bool Scene::should_act() {
    if (is_start_counting) {
        return start_time <= 0;
    }

    return false;
}

bool Scene::should_finish() {
    return false;
}

std::vector<Scene*> Scene::finish() {
    return {};
}


Scene* Scene::play_after(Scene* other) {
    anchor = other;
    anchor_type = PLAYING_AFTER;
    start_time = 0;
    return this;
}
Scene* Scene::play_with(Scene* other) {
    anchor = other;
    anchor_type = PLAYING_WITH;
    start_time = 0;
    return this;
}
Scene*  Scene::at_seconds(float time_seconds) {
    return this->at_frames(time_seconds * 60);
}
Scene*  Scene::at_frames(int time_frames) {
    start_time = time_frames;
    return this;
}
Scene* Scene::in_seconds(float time_frames) {
    return this->in_frames(time_frames * 60);
}
Scene* Scene::in_frames(int time_frames) {
    start_time += time_frames;
    return this;
}