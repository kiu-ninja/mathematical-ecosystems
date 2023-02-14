#pragma once

#include "stage/scene/scene.hpp"
#include "stage/scene/composition.hpp"
#include "stage/scene/controller/infinite.hpp"

namespace Scene {
    class Builder: public Scene {
    private:
        std::function<Scene*()> builder;
        
    public:
        Builder(std::function<Scene*()> f) {
            builder = f;
        }

        Scene* finish() override {
            return this;
        }

        void act() override {}

        Scene* begin() override {
            Scene* res = builder();
            return res;
        }
    };
};