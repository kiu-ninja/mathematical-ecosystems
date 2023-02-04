#pragma once
#include <iostream>
#include <vector>
#include "application.hpp"

template <typename S> class ApplicationWithState: public Application {
    public:
    S state;
    ApplicationWithState(int window_width, int window_height, const char* name) : 
        Application(window_width, window_height, name) {}
};

struct StatelessScene {
    int start_frame = 0;
    int duration_frames = 1;

    ApplicationData app_data;

    StatelessScene() {}
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
    
    virtual void _start(Application* application) {
        app_data = application->app_data;

        start();
    };

    StatelessScene* after(StatelessScene* other) {
        // start_frame = other->start_frame + other->duration_frames;
        this->set_start_frames(other->start_frame + other->duration_frames);
        return this;
    }

    StatelessScene* with(StatelessScene* other) {
        this->set_start_frames(other->start_frame);
        this->set_duration_frames(other->duration_frames);
        return this;
    }

    StatelessScene* wait(const float &delay_seconds) {
        this->set_start_frames(start_frame + delay_seconds * 60);
        return this;
    }

    virtual StatelessScene* set_duration(const float &duration_seconds) {
        duration_frames = duration_seconds * 60;
        return this;
    }

    virtual StatelessScene* set_duration_frames(const int &_duration_frames) {
        duration_frames = _duration_frames;
        return this;
    }

    virtual StatelessScene* set_start(const float &start_seconds) {
        start_frame = start_seconds * 60;
        return this;
    }

    virtual StatelessScene* set_start_frames(const int &_start_frame) {
        start_frame = _start_frame;
        return this;
    }

    virtual void _update_state(Application* application) {
        float t = (float)(application->current_frame - start_frame) / (duration_frames - 1);
    
        update_state(t);
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

    void _start(Application* application) override {
        app_data = application->app_data;

        start();
        start(((ApplicationWithState<S>*)application)->state);
    };

    void _update_state(Application* application) override {
        const float t = (float)(application->current_frame - start_frame) / (duration_frames - 1);
    
        update_state(t);
        update_state(*(ApplicationWithState<S>*)application, t);
    }
    
    virtual void update_state(ApplicationWithState<S> &application, const float &t) {
        // std::cout << "update_state() NOT IMPLEMENTED\n";
    }

    virtual void update_state(const float &t) override {
        // std::cout << "update_state() NOT IMPLEMENTED\n";
    };

    virtual void start(S &state) {
        // std::cout << "start() NOT IMPLEMENTED\n";
    }

    virtual void start() override {
        // std::cout << "start() NOT IMPLEMENTED\n";
    }
};

struct SceneGroup: public StatelessScene {
    using StatelessScene::StatelessScene;

    std::vector<StatelessScene*> scenes;

    SceneGroup* add(StatelessScene* scene) {
        scene->set_duration_frames(this->duration_frames);
        scene->set_start_frames(this->start_frame);
        scenes.push_back(scene);
        return this;
    }

    StatelessScene* set_duration(const float &duration_seconds) override {
        this->duration_frames = duration_seconds * 60;
        for (StatelessScene* scene : scenes) scene->set_duration(duration_seconds);
        return this;
    }

    StatelessScene* set_duration_frames(const int &_duration_frames) override {
        duration_frames = _duration_frames;
        for (StatelessScene* scene : scenes) scene->set_duration_frames(_duration_frames);
        return this;
    }

    StatelessScene* set_start(const float &start_seconds) override {
        start_frame = start_seconds * 60;
        for (StatelessScene* scene : scenes) scene->set_start(start_seconds);
        return this;
    }

    StatelessScene* set_start_frames(const int &_start_frame) override {
        start_frame = _start_frame;
        for (StatelessScene* scene : scenes) scene->set_start_frames(_start_frame);
        return this;
    }

    void _start(Application* application) override {
        for (StatelessScene* scene : scenes) {
            scene->_start(application);
        }
    }

    void _update_state(Application* application) override {
        for (StatelessScene* scene : scenes) {
            scene->_update_state(application);
        }
    }
};

// ================

template <typename S> class Stage: public ApplicationWithState<S> {
public:
    std::vector<StatelessScene*> scenes;

public:
    Stage(int window_width, int window_height, const char* name) : 
        ApplicationWithState<S>(window_width, window_height, name) {}

    void setup() {
        this->state = S(this->app_data);
        state_setup();
        scene_setup();
    }

    void update() {
        for (StatelessScene* s : scenes) {
            if (s->start_frame == this->current_frame) {
                s->_start(this);
            }

            if (s->start_frame <= this->current_frame && this->current_frame - s->start_frame < s->duration_frames) {
                s->_update_state(this);
            }
        }

        background_update();
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
        scenes.push_back(scene);
        return scene;
    }

    StatelessScene* last_scene() {
        return scenes[scenes.size() - 1];
    }

    /// 

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