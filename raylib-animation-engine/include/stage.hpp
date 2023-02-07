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
    int duration_frames = 2;

    ApplicationData app_data;
    Application* app; 

    virtual void _set_application_pointer(Application* application) {
        app = application;
    }

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

    virtual StatelessScene* get_scene() {
        _start();
        return this;
    }
    
    virtual void _start() {
        app_data = app->app_data;

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
        return this->wait_frames(delay_seconds * 60);
    }

    virtual StatelessScene* wait_frames(const float &delay_seconds) {
        this->set_start_frames(start_frame + delay_seconds);
        return this;
    }

    virtual StatelessScene* set_duration(const float &duration_seconds) {
        return this->set_duration_frames(duration_seconds * 60);
    }

    virtual StatelessScene* set_duration_frames(const int &_duration_frames) {
        duration_frames = _duration_frames;
        return this;
    }

    virtual StatelessScene* set_start(const float &start_seconds) {
        return this->set_start_frames(start_seconds * 60);
    }

    virtual StatelessScene* set_start_frames(const int &_start_frame) {
        start_frame = _start_frame;
        return this;
    }
    
    virtual void _update_state() {
        if (this->start_frame <= app->current_frame && app->current_frame - this->start_frame < this->duration_frames) {
            float t = (float)(app->current_frame - start_frame + 1) / (duration_frames);
        
            update_state(t);
        }
    }

    virtual void update_state(const float &t) { };
    virtual void start() { };
};

struct DebugScene: public StatelessScene {
    std::string s;
    DebugScene(std::string _s) {
        s = _s;
    }

    void start() override {
        std::cout << s << " | START" << "\n";
    }

    void update_state(const float &t) override {
        std::cout << s << " | UPDATE" << "\n";
    }
};

template <typename S> struct Scene: public StatelessScene {
    S initial_state;

    using StatelessScene::StatelessScene;

    void _start() override  {
        app_data = this->app->app_data;

        start();
        start(((ApplicationWithState<S>*)this->app)->state);
        _update_state();
    };

    void _update_state() override {
        if (this->start_frame < this->app->current_frame && this->app->current_frame - this->start_frame < this->duration_frames) {
            const float t = (float)(this->app->current_frame - start_frame + 1) / (duration_frames);
        
            update_state(t);
            update_state((ApplicationWithState<S>*)app, t);
        }
    }
    
    virtual void update_state(ApplicationWithState<S>* application, const float &t) { };
    virtual void update_state(const float &t) override { };
    virtual void start(S &state) { };
    virtual void start() override { };
};

struct SceneGroup: public StatelessScene {
    using StatelessScene::StatelessScene;

    std::vector<StatelessScene*> scenes;
    std::vector<bool> merged;

    void _set_application_pointer(Application* application) override {
        app = application;

        for (int i = 0; i < scenes.size(); i++) {
            scenes[i]->_set_application_pointer(application);
        }
    }

    SceneGroup(std::vector<StatelessScene*> _scenes) {
        for (StatelessScene* scene : _scenes) {
            merge(scene);
        }
    }

    SceneGroup* merge(StatelessScene* scene) {
        scene->set_duration_frames(this->duration_frames);
        scene->set_start_frames(this->start_frame);
        merged.push_back(true);
        return insert(scene);
    }


    SceneGroup* insert(StatelessScene* scene) {
        scene->_set_application_pointer(this->app);

        if (merged.size() == scenes.size()) {
            scene->set_start_frames(start_frame);
            merged.push_back(false);
        }

        scenes.push_back(scene);
        return this;
    }

    SceneGroup* set_duration_frames(const int &_duration_frames) override {
        for (int i = 0; i < scenes.size(); i++) {
            if (merged[i])
                scenes[i]->set_duration_frames(_duration_frames);
        }

        duration_frames = _duration_frames;
        return this;
    }

    SceneGroup* set_duration(const float &duration_seconds) override {
        return set_duration_frames(duration_seconds * 60);
    }

    SceneGroup* set_start(const float &start_seconds) override {
        return set_start_frames(start_seconds * 60);
    }

    SceneGroup* set_start_frames(const int &_start_frame) override {
        int d = _start_frame - start_frame;        
        start_frame = _start_frame;
        for (StatelessScene* scene : scenes) scene->wait_frames(d);
        return this;
    }

    StatelessScene* get_scene() override {
        for (StatelessScene* &scene : scenes) {
            scene = scene->get_scene();
        }

        _start();
        return this;
    }

    void _start() override {
        this->start();
    }

    void _update_state() override {
        for (StatelessScene* scene : scenes) {
            scene->_update_state();
        }
    }
};

struct SceneBuilder: public StatelessScene {
    std::function<void(SceneGroup*)> builder;
    
    SceneBuilder(std::function<void(SceneGroup*)> f) {
        builder = f;
    }

    StatelessScene* get_scene() override {
        SceneGroup* sg = new SceneGroup();
        sg->set_duration_frames(this->duration_frames);
        sg->set_start_frames(this->start_frame);
        sg->_set_application_pointer(this->app);
        builder(sg);
        return sg->get_scene();
    }
};

// ================

template <typename S> class Stage: public ApplicationWithState<S> {
public:
    std::vector<StatelessScene*> scenes;
    bool last_scene_is_a_group = false;

public:
    Stage(int window_width, int window_height, const char* name) : 
        ApplicationWithState<S>(window_width, window_height, name) {}

    void setup() {
        this->state = S(this->app_data);
        state_setup();
        scene_setup();
    }

    void update() {
        for (StatelessScene* &s : scenes) {
            if (s->start_frame == this->current_frame)
                s = s->get_scene();

            s->_update_state();
        }

        background_update();
    }

    StatelessScene* add_scene_after_last(StatelessScene* scene) {
        if (scenes.size() == 0)
            return add_scene(scene);

        return add_scene(scene->after(last_scene()));
    }

    SceneGroup* merge_scene_with_last(StatelessScene* scene) {
        if (last_scene_is_a_group) {
            ((SceneGroup *)last_scene())->merge(scene);
        } else {
            SceneGroup* sg = (SceneGroup *)(new SceneGroup())->with(last_scene());
            sg->_set_application_pointer(this);
            sg->merge(last_scene());
            sg->merge(scene);
            scenes[scenes.size() - 1] = sg;
            last_scene_is_a_group = true;
        }
        return (SceneGroup*)last_scene();
    }

    SceneGroup* add_scene_to_last(StatelessScene* scene) {
        SceneGroup* sg;
        if (last_scene_is_a_group) {
            sg = (SceneGroup *)last_scene();
            sg->insert(scene);
        } else {
            sg = new SceneGroup();//->with(last_scene());
            sg->_set_application_pointer(this);
            sg->insert(last_scene());
            sg->insert(scene);
            scenes[scenes.size() - 1] = sg;
            last_scene_is_a_group = true;
        }
        return sg;
    }

    SceneBuilder* add_scene_builder(std::function<void(SceneGroup*)> f) {
        SceneBuilder* sb = new SceneBuilder(f);
        add_scene_after_last(sb);
        return sb;
    }

    StatelessScene* add_scene(StatelessScene* scene) {
        last_scene_is_a_group = false;
        scene->_set_application_pointer(this);
        scenes.push_back(scene);
        return scene;
    }

    SceneGroup* new_scene_group() {
        SceneGroup* sg = new SceneGroup();
        add_scene_after_last(sg);
        last_scene_is_a_group = true;
        return sg;
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