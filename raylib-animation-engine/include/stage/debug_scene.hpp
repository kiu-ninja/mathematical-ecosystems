#include "stage/scene.hpp"
#include "stage/scene_controller/timed.hpp"

struct DebugScene: public Scene {
    std::string s;
    DebugScene(std::string _s) {
        s = _s;
    }

    Scene* begin() override {
        std::cout << s << " | BEGIN" << "\n";
        return this;
    }

    void act() override {
        std::cout << s << " | ACT" << "\n";
    }

    Scene* finish() override {
        std::cout << s << " | FINISH" << "\n";
        return this;
    }
};

DebugScene* new_debug_scene(std::string s) {
    DebugScene* ds = new DebugScene(s);
    ds->set_scene_controller(new TimedSceneController(0, 1));
    return ds;
}