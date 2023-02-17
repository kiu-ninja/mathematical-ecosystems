#include <cstdlib>

#pragma once

// Returns a random float in range 0..1
inline float randf() {
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

inline float randf(const float &min, const float &max) {
  return randf() * (max - min) + min;
}

inline int randi(const int &p_min, const int &p_max) {
  return rand() % (p_max - p_min) + p_min;
}

template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
  return v < lo ? lo : hi < v ? hi : v;
}
