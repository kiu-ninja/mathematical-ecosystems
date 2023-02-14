#include "stage/scene/builders.hpp"

namespace Scene {
    Builder* Builders::waiting_for(Scene* scene, Controller* waiting_for) {
        Builder* sb = new Builder([=](){
            return scene;
        });
        
        sb->set_scene_controller(new Controllers::Waiting(waiting_for));

        return sb;
    }

    InstantBuilder* Builders::instant(std::function<Scene *()> f)
    {
        return new InstantBuilder(f);
    }

    InstantExecutor* Builders::executor(std::function<void()> f)
    {
        return new InstantExecutor(f);
    }
}