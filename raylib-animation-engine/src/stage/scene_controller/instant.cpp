#include "stage/scene.hpp"

namespace Scene {
    namespace Controllers {
        void Instant::update() {
            if (current_frame >= 0)
                current_frame++;
            else
                current_frame = 0;
        }
        bool Instant::should_act() {
            return true;
        }
        bool Instant::should_finish() {
            return current_frame >= 1;
        }
        float Instant::get_t() {
            return 1.0f;
        }
    }
}