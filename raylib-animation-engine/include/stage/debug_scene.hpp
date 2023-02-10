#include "stage/scene.hpp"

struct DebugScene: public Scene {
    std::string s;
    DebugScene(std::string _s) {
        s = _s;
    }

    Scene* begin() override {
        std::cout << s << " | START" << "\n";
        return this;
    }

    void act() override {
        std::cout << s << " | UPDATE" << "\n";
    }

    std::vector<Scene*> finish() override {
        std::cout << s << " | END" << "\n";
        return {};
    }
};