#pragma once

#include "stage/scene/controller.hpp"

namespace Scene {
    namespace Controllers {
        class Instant: public Controller {
        public:
            void update() override;
            bool should_act() override;
            bool should_finish() override;
            float get_t() override;
        };
    };
};