#include "stage/scene.hpp"
#include "stage/scene_composition.hpp"

struct SceneBuilder: public Scene {
    std::function<std::vector<Scene*>()> builder;
    
    SceneBuilder(std::function<std::vector<Scene*>()> f) {
        builder = f;
    }

    std::vector<Scene*> finish() override {
        // SceneComposition* sg = new SceneComposition();
        // sg->duration_frames(this->duration);
        // sg->at_frames(this->start_time);
        std::vector<Scene*> res = builder();
        return builder();
    }
};