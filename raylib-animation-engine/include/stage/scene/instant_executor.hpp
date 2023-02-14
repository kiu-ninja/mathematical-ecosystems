#pragma once

#include "stage/scene/scene.hpp"
#include "stage/scene/composition.hpp"
#include "stage/scene/controller/instant.hpp"

namespace Scene {
    class InstantExecutor: public Scene {
    private:
        std::function<void()> f;

    public:
        InstantExecutor(std::function<void()> new_f) {
            f = new_f;
            set_scene_controller(new Controllers::Instant());
        }

        Scene* begin() override {
            return this;
        }

        void act() override { 
            f();
        }

        Scene* finish() override {
            return this;
        }
    };
};