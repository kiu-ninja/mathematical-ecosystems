#include "stage/scene.hpp"

namespace Scene {
    namespace Controllers {
        Waiting::Waiting(Controller* waiting_for) {
            this->handle = waiting_for;
            current_frame = -1;
        }

        void Waiting::update() {
            if (should_act()) {
                // current_frame = 0;
            }
        }
        bool Waiting::should_act() {
            return this->handle->should_finish();
        }
        bool Waiting::should_finish() {
            return false;
        }
        float Waiting::get_t() {
            return 1.0f;
        }   
    }
}