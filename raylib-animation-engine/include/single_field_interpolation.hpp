#include "stage/scene.hpp"
#include "easing.hpp"

namespace Interpolate {
    enum Mode { LINEAR, CUBIC, CUBIC_CUBIC, QUARDARIC };
    enum Behavior { RELATIVE_DELTA, RELATIVE_FACTOR, STATIC };

    template<typename T>
    struct EaseScene: public Scene::Scene { using Scene::Scene;
        T* object;
        T initial, target;
        Mode mode;
        Behavior behavior;
        float current = 0;

        Scene* begin() override {
            initial = *object;

            switch (behavior) {
                case STATIC: break;
                case RELATIVE_DELTA: target = *object + target; break;
                case RELATIVE_FACTOR: target = *object * target; break;
                default: break;
            }

            return this;
        }

        EaseScene(T* _object, T _target, Mode _mode, Behavior _behavior) {
            object = _object;
            target = _target;
            mode = _mode;
            behavior = _behavior;
        }

        void act() override {
            float t = get_scene_controller()->get_t();

            switch (mode) {
                case LINEAR:      t = Easing::linear(0, 1, t);      break;
                case CUBIC:       t = Easing::cubic(0, 1, t);       break;
                case CUBIC_CUBIC: t = Easing::cubic_cubic(0, 1, t); break;
                case QUARDARIC:   t = Easing::quadratic(0, 1, t);   break;
                default:         /*-------------------------------*/ break;
            }

            *object = *object + (target - initial) * (t - current);
            current = t;
        }

        Scene* finish() override {
            return this;
        }
    };

    template<typename T>
    EaseScene<T>* interpolate(T* object, T target, Mode mode, Behavior Behavior) {
        return new EaseScene<T>(object, target, mode, Behavior);
    }
}
