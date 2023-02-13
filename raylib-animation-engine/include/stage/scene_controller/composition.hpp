#pragma once

#include "stage/scene_composition.hpp"
#include "stage/scene_controller.hpp"

class SceneCompositionController: public SceneController {
private:
    std::vector<Scene*>& handle;
public:
    SceneCompositionController(std::vector<Scene*> &sc);
    void update() override;
    bool should_act() override;
    bool should_finish() override;
    float get_t() override;
};