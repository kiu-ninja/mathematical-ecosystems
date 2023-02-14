#pragma once

#include "stage/scene/scene.hpp"
#include "stage/scene/controller.hpp"
#include "stage/scene/controller/timed.hpp"
#include "stage/scene/controller/waiting.hpp"
#include "stage/scene/controller/infinite.hpp"
#include "stage/scene/controller/instant.hpp"


namespace Scene {
    namespace Controllers {
        Scene* timed(Scene* scene, int start_time, int duration);
        Scene* timed(Scene* scene, float start_time, float duration);
        Scene* instant(Scene* scene);
        Scene* infinite(Scene* scene);
    }
}