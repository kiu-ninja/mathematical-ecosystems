#include "stage.hpp"
#include "easing.hpp"

namespace Interpolate {
    enum Mode { LINEAR, CUBIC, CUBIC_CUBIC, QUARDARIC };
    enum Behavior { RELATIVE_DELTA, RELATIVE_FACTOR, STATIC };

    template<typename T>
    struct EaseScene: public StatelessScene { using StatelessScene::StatelessScene;
        T* object;
        T initial, target, current;
        Mode mode;
        Behavior behavior;

        virtual void start() {
            initial = *object;

            switch (behavior) {
                case STATIC: break;
                case RELATIVE_DELTA: target = *object + target; break;
                case RELATIVE_FACTOR: target = *object * target; break;
                default: break;
            }

            current = target - target;
        }

        EaseScene(T* _object, T _target, Mode _mode, Behavior _behavior) {
            object = _object;
            target = _target;
            mode = _mode;
            behavior = _behavior;

            this->start_frame = 0;
            this->duration_frames = 60;
        }

        void update_state(const float &t) {
            float _t = 0;

            switch (mode) {
                case LINEAR:      _t = Easing::linear(0, 1, t);      break;
                case CUBIC:       _t = Easing::cubic(0, 1, t);       break;
                case CUBIC_CUBIC: _t = Easing::cubic_cubic(0, 1, t); break;
                case QUARDARIC:   _t = Easing::quadratic(0, 1, t);   break;
                default:         /*-------------------------------*/ break;
            }

            T next = _t * (target - initial);
            *object = *object + (next - current);
            current = next;
        }
    };

    template<typename T>
    EaseScene<T>* interpolate(T* object, T target, Mode mode, Behavior Behavior) {
        return new EaseScene<T>(object, target, mode, Behavior);
    }
}
