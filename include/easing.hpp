#pragma once


namespace Easing {
    inline float linear(const float &a, const float &b, const float &t) {
        return a + (b - a) * t;
    }

    void linear_in_place(float &a, const float &b, const float &t) {
        a += (b - a) * t;
    }

    inline float cubic(const float &a, const float &b, const float &t) {
        return linear(a, b, 3 * t * t - 2 * t * t * t);
    }

    inline float cubic_cubic(const float &a, const float &b, float t) {
        for (int i = 0; i < 2; i++)
            t = 3 * t * t - 2 * t * t * t;
        
        return linear(a, b, t);
    }

    inline float quadratic(const float &a, const float &b, float t) {
        t = t * (2 - t);

        return linear(a, b, t);
    }
}