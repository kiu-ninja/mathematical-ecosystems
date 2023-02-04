#pragma once
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
struct StatelessScene {
    int start_frame = 0;
    int duration_frames = 1;

    ApplicationData app_data;

    StatelessScene(const float &duration_seconds) {
        // Initialize a scene that will last `duration_seconds`
        if (duration_seconds > 0) {
            duration_frames = duration_seconds * 60;
        } else {
            duration_frames = 1 << 30;
        }
    }

    StatelessScene(const float &start_seconds, const float &duration_seconds) {
        // Initialize a scene that will start at `start_seconds` and will last `duration_seconds`
        start_frame = start_seconds * 60;
        if (duration_seconds > 0) {
            duration_frames = duration_seconds * 60;
        } else {
            duration_frames = 1 << 30;
        }
    }
    
    void _start(const ApplicationData &_app_data) {
        app_data = _app_data;

        start();
    };

    StatelessScene* after(StatelessScene* other) {
        start_frame = other->start_frame + other->duration_frames;
        return this;
    }

    StatelessScene* with(StatelessScene* other) {
        start_frame = other->start_frame;
        return this;
    }

    StatelessScene* wait(const float &delay_seconds) {
        start_frame += delay_seconds * 60;
        return this;
    }

    StatelessScene* set_duration(const float &duration_seconds) {
        duration_frames = duration_seconds * 60;
        return this;
    }

    virtual void update_state(const float &t) {
        // std::cout << "update_state() NOT IMPLEMENTED\n";
    };

    virtual void start() {
        // std::cout << "start() NOT IMPLEMENTED\n";
    }
};

template <typename S> struct Scene: public StatelessScene {
    S initial_state;

    using StatelessScene::StatelessScene;
    
    void _start(const ApplicationData &_app_data, S &state) {
        initial_state = state;
        app_data = _app_data;

        start();
        start(state);
    };

    virtual void update_state(S &state, const float &t) {
        // std::cout << "update_state() NOT IMPLEMENTED\n";
    };

    virtual void update_state(const float &t) {
        // std::cout << "update_state() NOT IMPLEMENTED\n";
    };

    virtual void start(S &state) {
        // std::cout << "start() NOT IMPLEMENTED\n";
    }

    virtual void start() {
        // std::cout << "start() NOT IMPLEMENTED\n";
    }
};

// ================

template <typename S> class Stage: public Application {
public:
    S state;
    int current_frame;
    std::vector<Scene<S>*> scenes;
    std::vector<StatelessScene*> stateless_scenes;
    StatelessScene* _last_scene;

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
            }

            if (s->start_frame <= current_frame && current_frame - s->start_frame < s->duration_frames) {
                s->update_state(state, (float)(current_frame - s->start_frame) / (s->duration_frames - 1));
                s->update_state((float)(current_frame - s->start_frame) / (s->duration_frames - 1));
            }
        }

        for (StatelessScene* s : stateless_scenes) {
            if (s->start_frame == current_frame) {
                s->_start(app_data);
            }

            if (s->start_frame <= current_frame && current_frame - s->start_frame < s->duration_frames) {
                s->update_state((float)(current_frame - s->start_frame) / (s->duration_frames - 1));
            }
        }

        current_frame++;
        background_update();
    }

    std::vector<Scene<S>*> add_scenes_after_last(const std::vector<Scene<S>*> &_scenes) {
        Scene<S>* last = last_scene();
        for (Scene<S>* scene : _scenes) {
            add_scene(scene->after(last))->set_duration(last->duration_frames / 60);
        }
        return _scenes;
    }

    std::vector<Scene<S>*> add_scenes_with_last(const std::vector<Scene<S>*> &_scenes) {
        for (Scene<S>* scene : _scenes) {
            add_scene_with_last(scene)->set_duration(last_scene()->duration_frames / 60);
        }
        return _scenes;
    }

    Scene<S>* add_scene_after_last(Scene<S>* scene) {
        add_scene(scene->after(last_scene()));
        return scene;
    }

    Scene<S>* add_scene_with_last(Scene<S>* scene) {
        add_scene(scene->with(last_scene()));
        return scene;
    }

    Scene<S>* add_scene(Scene<S>* scene) {
        scenes.push_back(scene);
        _last_scene = scene;
        return scene;
    }

    std::vector<StatelessScene*> add_scenes_after_last(const std::vector<StatelessScene*> &_scenes) {
        StatelessScene* last = last_scene();
        for (StatelessScene* scene : _scenes) {
            add_scene(scene->after(last))->set_duration(last->duration_frames / 60);
        }
        return _scenes;
    }

    std::vector<StatelessScene*> add_scenes_with_last(const std::vector<StatelessScene*> &_scenes) {
        for (StatelessScene* scene : _scenes) {
            add_scene_with_last(scene)->set_duration(last_scene()->duration_frames / 60);
        }
        return _scenes;
    }

    StatelessScene* add_scene_after_last(StatelessScene* scene) {
        add_scene(scene->after(last_scene()));
        return scene;
    }

    StatelessScene* add_scene_with_last(StatelessScene* scene) {
        add_scene(scene->with(last_scene()));
        return scene;
    }

    StatelessScene* add_scene(StatelessScene* scene) {
        stateless_scenes.push_back(scene);
        _last_scene = scene;
        return scene;
    }

    StatelessScene* last_scene() {
        return _last_scene;
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