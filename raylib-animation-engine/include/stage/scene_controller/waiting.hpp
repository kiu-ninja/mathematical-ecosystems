#pragma once

#include "stage/scene_controller.hpp"

class WaitingSceneController: public SceneController {
private:
    SceneController* handle;

public:
    WaitingSceneController(SceneController* waiting_for);

    void update() override;
    bool should_act() override;
    bool should_finish() override;
    float get_t() override;
};