#pragma once

#include "stage/scene/controller.hpp"

namespace Scene {
    namespace Controllers {
        class Waiting: public Controller {
        private:
            Controller* handle;

        public:
            Waiting(Controller* waiting_for);

            void update() override;
            bool should_act() override;
            bool should_finish() override;
            float get_t() override;
        };
    };
};