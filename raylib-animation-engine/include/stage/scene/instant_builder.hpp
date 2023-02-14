#pragma once

#include "stage/scene/scene.hpp"
#include "stage/scene/composition.hpp"
#include "stage/scene/controller/instant.hpp"

namespace Scene {
    /*
    Recieves a lambda which returns a new scene. Once this scene `begin`s, it will call the passed in lambda, set the resulting scenes controller to its own, and return it.
    */
    class InstantBuilder: public Scene {
    private:
        std::function<Scene*()> f;

    public:
        InstantBuilder(std::function<Scene*()> new_f) {
            f = new_f;
            set_scene_controller(new Controllers::Instant());
        }

        Scene* begin() override {
            Scene* res = f();
            res->set_scene_controller(get_scene_controller());
            return res;
        }

        void act() override { }

        Scene* finish() override {
            return this;
        }
    };
};