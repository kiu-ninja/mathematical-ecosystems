#pragma once

#include "stage/scene_controller.hpp"

class InfiniteSceneController: public SceneController {
public:
    void update() override;
    bool should_act() override;
    bool should_finish() override;
    float get_t() override;
};