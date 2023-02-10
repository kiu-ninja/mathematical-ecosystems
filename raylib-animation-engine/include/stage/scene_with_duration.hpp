#pragma once
#include "stage/scene.hpp"

class TimedScene: virtual public Scene {
protected:
    int duration = 1;

public:

    virtual int get_duration();
    virtual void update() override;
    virtual bool should_act() override;
    virtual bool should_finish() override;

    /* Sets the duration of the scene to time_seconds. */
    TimedScene*  duration_seconds(float time_seconds);
    /* Sets the duration of the scene to time_frames. */
    TimedScene*  duration_frames(int time_frames);
    /* Makes the start time and duration same as other_scene. */
    TimedScene*  play_with(TimedScene* other_scene);
    /* Sets the start time of the scene to when the other_scene ends. */
    TimedScene*  play_after(TimedScene* other_scene);
};