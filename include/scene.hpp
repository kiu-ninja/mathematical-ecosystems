

template <typename S> struct Scene {
    S initial_state;
    
    int start_frame;
    int duration_frames;

    Stage<S>* stage;

    Scene(const float &start_seconds, const float &duration_seconds) {
        // Initialize a scene that will start at `start_seconds` and will last `duration_seconds`
        _set_start_and_duration_frames(start_seconds * 60, duration_seconds * 60);
    }
    
    Scene(Scene<S>* previous_scene, const float &start_seconds, const float &duration_seconds) {
        // Initialize a scene that will start `start_seconds` *after* the `previous_scene`
        _set_start_and_duration_frames(previous_scene->duration_frames + previous_scene->start_frame + start_seconds * 60, duration_seconds * 60);
    }
    
    Scene(Scene<S>* previous_scene, const float &duration_seconds) {
        // Initialize a scene that will start *right after* the `previous_scene`
        _set_start_and_duration_frames(previous_scene->duration_frames + previous_scene->start_frame, duration_seconds * 60);
    }

    void _set_start_and_duration_frames(const float &_start_frame, const float &_duration_frames) {
        start_frame = _start_frame;
        if (_duration_frames > 0) {
            duration_frames = _duration_frames;
        } else {
            duration_frames = 1 << 30;
        }

        std::cout << "NEW SCENE: start=" << start_frame << " duration=" << duration_frames << "\n";
    }
    
    virtual void update_state(S &state, const float &t) {
        std::cout << "update_state() NOT IMPLEMENTED\n";
    };

    virtual void start(Stage<S>* _stage, const S state) {
        stage = _stage;
        initial_state = state;
        // std::cout << "start() NOT IMPLEMENTED\n";
    };
};