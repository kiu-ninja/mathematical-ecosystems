#include "stage/scene/scene.hpp"
#include "stage/scene/controller/timed.hpp"

namespace Scene {
    struct Debug: public Scene {
        std::string s;
        Debug(std::string _s) {
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
};

Scene::Debug* new_debug_scene(std::string s) {
    Scene::Debug* ds = new Scene::Debug(s);
    ds->set_scene_controller(new Scene::Controllers::Timed(0, 1));
    return ds;
}