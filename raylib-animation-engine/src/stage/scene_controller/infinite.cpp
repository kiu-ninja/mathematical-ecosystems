#include "stage/scene.hpp"

namespace Scene {
    namespace Controllers {
        void Infinite::update() {
            current_frame = 0;
        }
        bool Infinite::should_act() {
            return true;
        }
        bool Infinite::should_finish() {
            return false;
        }
        float Infinite::get_t() {
            return 0.016666f; // 1/60
        }
    }
}