#include <iostream>
#include <vector>
#include "application.hpp"


/*
```
// Example Scene:
struct ExampleScene: public Scene<ExampleState> { using Scene::Scene;
    int current_frame;

    void start(const ExampleState &s) {
        // s - Initial state object

        current_frame = 0;
    }

    void update_state(ExampleState &s, const float &t) {
        // s - A reference to the current state object (you should modift this object)
        // t - Percentage way through the scene (0..1)

        s.some_random_field += current_frame;

        current_frame++;
    }
}
```
*/
template <typename S> struct Scene {
    S initial_state;
    
    int start_frame = 0;
    int duration_frames = 1;

    ApplicationData app_data;

    Scene(const float &duration_seconds) {
        // Initialize a scene that will last `duration_seconds`
        if (duration_seconds > 0) {
            duration_frames = duration_seconds * 60;
        } else {
            duration_frames = 1 << 30;
        }
    }

    Scene(const float &start_seconds, const float &duration_seconds) {
        // Initialize a scene that will start at `start_seconds` and will last `duration_seconds`
        start_frame = start_seconds * 60;
        if (duration_seconds > 0) {
            duration_frames = duration_seconds * 60;
        } else {
            duration_frames = 1 << 30;
        }
    }
    
    void _start(const ApplicationData &_app_data, const S state) {
        initial_state = state;
        app_data = _app_data;

        start();
    };

    Scene<S>* after(Scene<S>* other) {
        start_frame = other->start_frame + other->duration_frames;
        return this;
    }

    Scene<S>* with(Scene<S>* other) {
        start_frame = other->start_frame;
        return this;
    }

    Scene<S>* wait(const float &delay_seconds) {
        start_frame += delay_seconds * 60;
        return this;
    }

    Scene<S>* set_duration(const float &duration_seconds) {
        duration_frames = duration_seconds * 60;
        return this;
    }

    virtual void update_state(S &state, const float &t) {
        std::cout << "update_state() NOT IMPLEMENTED\n";
    };

    virtual void start(S &state) {
        std::cout << "start() NOT IMPLEMENTED\n";
    }

    virtual void start() {
        std::cout << "start() NOT IMPLEMENTED\n";
    }
};

// ================

template <typename S> class Stage: public Application {
public:
    S state;
    int current_frame;
    std::vector<Scene<S>*> scenes;

public:
    Stage(int window_width, int window_height, const char* name) : 
        Application(window_width, window_height, name) {}

    void setup() {
        current_frame = 0;
        state = S(app_data);
        state_setup();
        scene_setup();
    }

    void update() {
        for (Scene<S>* s : scenes) {
            if (s->start_frame == current_frame) {
                s->_start(app_data, state);
                s->start(state);
            }

            if (s->start_frame <= current_frame && current_frame - s->start_frame < s->duration_frames) {
                s->update_state(state, (float)(current_frame - s->start_frame) / (s->duration_frames - 1));
            }
        }

        current_frame++;
        background_update();
    }

    Scene<S>* add_scene_after_last(Scene<S>* scene) {
        add_scene(scene->after(last_scene()));
        return scene;
    }

    Scene<S>* add_scene_with_last(Scene<S>* scene) {
        scene->duration_frames = last_scene()->duration_frames;
        add_scene(scene->with(last_scene()));
        return scene;
    }

    Scene<S>* add_scene(Scene<S>* scene) {
        scenes.push_back(scene);
        return scene;
    }

    Scene<S>* last_scene() {
        return scenes[scenes.size() - 1];
    }

    virtual void state_setup() { 
        // Override this method and initialize your state in there
        std::cout << "state_setup() NOT IMPLEMENTED\n";
    };
    virtual void scene_setup() { 
        // Override this method and initialize your scenes in there
        std::cout << "scene_setup() NOT IMPLEMENTED\n"; 
    };
    virtual void background_update() {
        // Override this method and do updating (not used for updating animations)
        std::cout << "background_update() NOT IMPLEMENTED\n";
    };
};