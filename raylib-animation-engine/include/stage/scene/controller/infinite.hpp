#pragma once

#include "stage/scene/controller.hpp"

namespace Scene {
    namespace Controllers {
        class Infinite: public Controller {
        public:
            void update() override;
            bool should_act() override;
            bool should_finish() override;
            float get_t() override;
        };
    };
};