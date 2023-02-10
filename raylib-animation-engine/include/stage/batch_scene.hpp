#pragma once

#include <vector>
#include "stage/scene.hpp"

class BatchScene: virtual public Scene {
private:
    std::vector<Scene*> scenes;
    int number_of_scenes;

public:
    BatchScene() {
        current_frame = 0;
        number_of_scenes = 0;
    }

    virtual void  act() override;

    /* Inserts the scene at a given arbitrary time. Returns the scene passed in. */
    Scene*  add(Scene* scene);
};