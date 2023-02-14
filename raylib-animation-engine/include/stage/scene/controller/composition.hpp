#pragma once

#include "stage/scene/composition.hpp"
#include "stage/scene/controller.hpp"


namespace Scene {
    namespace Controllers {
        class Composition: public Controller {
        private:
            std::vector<Scene*>& handle;
        public:
            Composition(std::vector<Scene*> &sc);
            void update() override;
            bool should_act() override;
            bool should_finish() override;
            float get_t() override;
        };
    };
};