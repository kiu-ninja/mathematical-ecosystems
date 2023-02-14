#include "stage/scene/scene.hpp"
#include "stage/scene/batch.hpp"
#include "stage/scene/builder.hpp"
#include "stage/scene/instant_builder.hpp"
#include "stage/scene/instant_executor.hpp"
#include "stage/scene/controller.hpp"
#include "stage/scene/controller/timed.hpp"
#include "stage/scene/controller/waiting.hpp"

namespace Scene {
    namespace Builders {
        Builder* waiting_for(Scene* scene, Controller* waiting_for);
        InstantBuilder* instant(std::function<Scene*()> f);
        InstantExecutor* executor(std::function<void()> f);
    }
}
