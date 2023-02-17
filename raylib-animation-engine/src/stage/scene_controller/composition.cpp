#include "stage/scene.hpp"

namespace Scene {
    namespace Controllers {
        Composition::Composition(std::vector<Scene*> &sc) : handle(sc) {
            // handle = sc;
        }
        void Composition::update() {
            current_frame = 0;
        }
        bool Composition::should_act() {
            return true;
        }
        bool Composition::should_finish() {
            for (Scene* scene : handle) {
                if (!scene->get_scene_controller()->should_finish()) {
                    return false;
                }
            }
            return true;
        }
        float Composition::get_t() {
            return 1.0f;
        }
    }
}