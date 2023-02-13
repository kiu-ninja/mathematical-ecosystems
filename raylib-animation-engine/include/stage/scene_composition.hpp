#pragma once

#include <vector>
#include "stage/scene.hpp"

class SceneComposition: public Scene {
private:
    std::vector<Scene*> scenes;
    int number_of_scenes = 0;

public:
    SceneComposition();

    virtual Scene*  begin() override;
    virtual void  act() override;
    virtual Scene*  finish() override;

    /* Inserts the scene at a given arbitrary time. Returns the scene passed in. */
    Scene*  add(Scene* scene);
    /* Returns the last-inserted scene. */
    Scene*  last_scene();
};