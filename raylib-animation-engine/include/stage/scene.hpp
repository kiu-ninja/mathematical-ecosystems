#pragma once

#include <vector>

#include "application/application.hpp"

namespace Scene {
class Controller {
protected:
    int current_frame = -1;

public:
    /* Updates the state. Should be called only once per frame. */
    virtual void update() = 0;
    /* Should be called only once per frame. */
    virtual bool should_act() = 0;
    /* Should be called only once per frame. */
    virtual bool should_finish() = 0;

    // NOTE: Is this a good idea?
    bool has_begun() { return current_frame >= 0; };

    // NOTE: IS THIS A GOOD IDEA?
    virtual float get_t() = 0;
};

class Scene {
protected:
    Controller* scene_controller;
    bool has_scene_controller;

public:
    Controller* get_scene_controller();
    virtual void set_scene_controller(Controller* new_scene_controller);

    /* Called at the first frame of the scene. */
    virtual Scene* begin() = 0;
    /* Called at every frame of the scene. */
    virtual void act() = 0;
    /* Called at the last frame of the scene. Returns a vector of scenes that
     * should be added to the active scene after the scene finishes. */
    virtual Scene* finish() = 0;
};

namespace Controllers {
    Scene* timed(Scene* scene, int start_time, int duration);
    Scene* timed(Scene* scene, float start_time, float duration);
    Scene* instant(Scene* scene);
    Scene* infinite(Scene* scene);

    class Composition : public Controller {
    private:
        std::vector<Scene*>& handle;

    public:
        Composition(std::vector<Scene*>& sc);
        void update() override;
        bool should_act() override;
        bool should_finish() override;
        float get_t() override;
    };

    class Infinite : public Controller {
    public:
        void update() override;
        bool should_act() override;
        bool should_finish() override;
        float get_t() override;
    };

    class Instant : public Controller {
    public:
        void update() override;
        bool should_act() override;
        bool should_finish() override;
        float get_t() override;
    };

    class Timed : public Controller {
    private:
        int start_time;
        int duration;

    public:
        Timed(float start_time_seconds, float duration_seconds);
        Timed(int start_time_frames, int duration_frames);

        /* Sets the start time of the scene to time_seconds. */
        void at_seconds(float time_seconds);
        /* Sets the start time of the scene to time_frames. */
        void at_frames(int time_frames);
        /* Delays the scene with time_seconds. */
        void in_seconds(float time_seconds);
        /* Delays the scene with time_frames. */
        void in_frames(int time_frames);
        /* Sets the duration of the scene to time_seconds. */
        void for_seconds(float time_seconds);
        /* Sets the duration of the scene to time_frames. */
        void for_frames(int time_frames);

        void update() override;
        bool should_act() override;
        bool should_finish() override;
        float get_t() override;
    };

    class Waiting : public Controller {
    private:
        Controller* handle;

    public:
        Waiting(Controller* waiting_for);

        void update() override;
        bool should_act() override;
        bool should_finish() override;
        float get_t() override;
    };
} // namespace Controllers

class Batch : public Scene {
private:
    std::vector<Scene*> scenes;

public:
    void set_scene_controller(Controller* new_scene_controller) override;

    virtual Scene* begin() override;
    virtual void act() override;
    virtual Scene* finish() override;

    /* Inserts the scene at a given arbitrary time. Returns the scene passed in.
     */
    void add(Scene* scene);
};

class Composition : public Scene {
private:
    std::vector<Scene*> scenes;
    int number_of_scenes = 0;

public:
    Composition();

    virtual Scene* begin() override;
    virtual void act() override;
    virtual Scene* finish() override;

    /* Inserts the scene at a given arbitrary time. Returns the scene passed in.
     */
    Scene* add(Scene* scene);
    /* Returns the last-inserted scene. */
    Scene* last_scene();
};

struct Debug : public Scene {
    std::string s;
    Debug(std::string _s) { s = _s; }

    Scene* begin() override
    {
        std::cout << s << " | BEGIN"
                  << "\n";
        return this;
    }

    void act() override
    {
        std::cout << s << " | ACT"
                  << "\n";
    }

    Scene* finish() override
    {
        std::cout << s << " | FINISH"
                  << "\n";
        return this;
    }
};

class InstantExecutor : public Scene {
private:
    std::function<void()> f;

public:
    InstantExecutor(std::function<void()> new_f)
    {
        f = new_f;
        set_scene_controller(new Controllers::Instant());
    }

    Scene* begin() override { return this; }

    void act() override { f(); }

    Scene* finish() override { return this; }
};

namespace Builders {
    class Builder : public Scene {
    private:
        std::function<Scene*()> builder;

    public:
        Builder(std::function<Scene*()> f) { builder = f; }

        Scene* finish() override { return this; }

        void act() override { }

        Scene* begin() override
        {
            Scene* res = builder();
            return res;
        }
    };

    /*
    Recieves a lambda which returns a new scene. Once this scene `begin`s, it will
    call the passed in lambda, set the resulting scenes controller to its own, and
    return it.
    */
    class Instant : public Scene {
    private:
        std::function<Scene*()> f;

    public:
        Instant(std::function<Scene*()> new_f)
        {
            f = new_f;
            set_scene_controller(new Controllers::Instant());
        }

        Scene* begin() override
        {
            Scene* res = f();
            res->set_scene_controller(get_scene_controller());
            return res;
        }

        void act() override { }

        Scene* finish() override { return this; }
    };

    Builder* waiting_for(Scene* scene, Controller* waiting_for);
    Instant* instant(std::function<Scene*()> f);
    InstantExecutor* executor(std::function<void()> f);
}; // namespace Builders
}; // namespace Scene
