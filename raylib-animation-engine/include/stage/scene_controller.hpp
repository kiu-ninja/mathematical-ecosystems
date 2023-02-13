#pragma once

class SceneController {
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