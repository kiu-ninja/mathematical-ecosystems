#include <iostream>
#include "stage/scene/controller/timed.hpp"

namespace Scene {
    namespace Controllers {
        Timed::Timed(float start_time_seconds, float duration_seconds) {
            at_seconds(start_time_seconds);
            for_seconds(duration_seconds);
            current_frame = -1;
        }
        Timed::Timed(int start_time_frames, int duration_frames) {
            at_frames(start_time_frames);
            for_frames(duration_frames);
            current_frame = -1;
        }

        void Timed::at_seconds(float time_seconds) {
            at_frames(time_seconds * 60);
        }
        void Timed::at_frames(int time_frames) {
            start_time = time_frames;
        }
        void Timed::for_seconds(float time_seconds) {
            for_frames(time_seconds * 60);
        }
        void Timed::for_frames(int time_frames) {
            duration = time_frames;
        }
        void Timed::in_seconds(float time_seconds) {
            in_frames(time_seconds * 60);
        }
        void Timed::in_frames(int time_frames) {
            start_time += time_frames;
        }

        void Timed::update() {
            if (should_act() && !should_finish()) {
                current_frame++;
            }

            if (!should_act()) {
                start_time--;
            }
        };
        bool Timed::should_act() {
            return this->start_time <= 0;
        };
        bool Timed::should_finish() {
            return this->current_frame + 1 >= this->duration;
        }
        float Timed::get_t() {
            return (float)this->current_frame / this->duration;
        }
    }
}