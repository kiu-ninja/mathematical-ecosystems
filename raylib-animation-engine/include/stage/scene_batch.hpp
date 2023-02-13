#pragma once

#include <vector>
#include "stage/scene.hpp"

class SceneBatch: public Scene {
private:
    std::vector<Scene*> scenes;

public:
    void set_scene_controller(SceneController* new_scene_controller) override;

    virtual Scene*  begin() override;
    virtual void  act() override;
    virtual Scene*  finish() override;

    /* Inserts the scene at a given arbitrary time. Returns the scene passed in. */
    void  add(Scene* scene);
};