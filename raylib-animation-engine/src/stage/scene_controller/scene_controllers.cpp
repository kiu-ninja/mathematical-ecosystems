#include "stage/scene/controllers.hpp"

namespace Scene {
    namespace Controllers {
        Scene* timed(Scene* scene, int start_time, int duration) {
            scene->set_scene_controller(new Timed(start_time, duration));

            return scene;
        }

        Scene* timed(Scene* scene, float start_time, float duration) {
            scene->set_scene_controller(new Timed(start_time, duration));

            return scene;
        }

        Scene* instant(Scene* scene) {
            scene->set_scene_controller(new Instant());

            return scene;
        }

        Scene* infinite(Scene* scene) {
            scene->set_scene_controller(new Infinite());

            return scene;
        }
    }
}