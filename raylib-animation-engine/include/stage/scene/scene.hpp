#pragma once

#include <vector>
#include "application/application.hpp"
#include "stage/scene/controller.hpp"

namespace Scene {
    class Scene {
    protected:
        Controller* scene_controller;
        bool has_scene_controller;

    public:
        Controller* get_scene_controller();
        virtual void set_scene_controller(Controller* new_scene_controller);

        /* Called at the first frame of the scene. */
        virtual Scene*  begin() = 0;
        /* Called at every frame of the scene. */
        virtual void  act() = 0;
        /* Called at the last frame of the scene. Returns a vector of scenes that should be added to the active scene after the scene finishes. */
        virtual Scene*  finish() = 0;
    };
};