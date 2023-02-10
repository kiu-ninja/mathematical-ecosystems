#pragma once
#include <vector>
#include "application/application.hpp"

class Scene {
protected:
    int start_time = 0;
    int current_frame = -1;


    enum AnchorStatus {
        NOT_ANCHORED, PLAYING_WITH, PLAYING_AFTER
    };

    Scene* anchor;
    AnchorStatus anchor_type = NOT_ANCHORED;
    bool is_start_counting = false;

public:
    /* */
    Scene() {}

    /* Returns the start time of the scene in frames. */
    int get_start_time();

    virtual void update();
    /* Called at the first frame of the scene. */
    virtual Scene*  begin();
    /* Returns true if the scene has already begun playing. */
    virtual bool  has_begun();

    /* Returns true at every frame since the start of the animation. */
    virtual bool should_act();
    /* Called at every frame of the scene. */
    virtual void  act() = 0;
    
    /* Returns true at every frame since the end of the animation. */
    virtual bool should_finish();
    /* Called at the last frame of the scene. Returns a vector of scenes that should be added to the active scene after the scene finishes. */
    virtual std::vector<Scene*>  finish();


    /* Sets the start time of the scene to time_seconds. */
    virtual Scene*  at_seconds(float time_seconds);
    /* Sets the start time of the scene to time_frames. */
    virtual Scene*  at_frames(int time_frames);
    /* Delays the scene with time_seconds. */
    virtual Scene*  in_seconds(float time_seconds);
    /* Delays the scene with time_frames. */
    virtual Scene*  in_frames(int time_frames);
    /* Makes the start time and duration same as other_scene. */
    virtual Scene*  play_with(Scene* other_scene);
    /* Sets the start time of the scene to when the other_scene ends. */
    virtual Scene*  play_after(Scene* other_scene);
};