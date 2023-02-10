#pragma once

#include <vector>
#include "stage/scene.hpp"

class SceneComposition: virtual public Scene {
private:
    std::vector<Scene*> scenes;
    int number_of_scenes;

public:
    SceneComposition() {
        current_frame = 0;
        number_of_scenes = 0;
    }

    virtual void  act() override;
    virtual bool  should_finish() override;

    /* Inserts the scene at a given arbitrary time. Returns the scene passed in. */
    Scene*  add(Scene* scene);
    /* Inserts the scene with the time parameters of the last-inserted scene. (i.e. the start time and the duration will match) */
    Scene*  add_with_last(Scene* scene);
    /* Inserts the scene at the time last-inserted scene finishes. */
    Scene*  add_after_last(Scene* scene);
    /* Returns the last-inserted scene. */
    Scene*  last_scene();
};