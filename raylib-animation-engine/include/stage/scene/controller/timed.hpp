#pragma once

#include "stage/scene/controller.hpp"

namespace Scene {
    namespace Controllers {
        class Timed: public Controller {
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
    };
};