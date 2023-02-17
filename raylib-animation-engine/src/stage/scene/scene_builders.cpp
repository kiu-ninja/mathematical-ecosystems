#include "stage/scene.hpp"

namespace Scene {
    Builders::Builder* Builders::waiting_for(Scene* scene, Controller* waiting_for) {
        Builders::Builder* sb = new Builders::Builder([=](){
            return scene;
        });
        
        sb->set_scene_controller(new Controllers::Waiting(waiting_for));

        return sb;
    }

    Builders::Instant* Builders::instant(std::function<Scene *()> f)
    {
        return new Instant(f);
    }

    InstantExecutor* Builders::executor(std::function<void()> f)
    {
        return new InstantExecutor(f);
    }
}