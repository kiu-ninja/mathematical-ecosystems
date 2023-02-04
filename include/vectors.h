#include <iostream>
#include <math.h>
#include <raylib.h>

#pragma once

inline bool operator==(const Vector2 &a, const Vector2 &b) {
  return a.x == b.x && a.y == b.y;
}

// Subtraction
inline Vector2 operator-(const Vector2 &a, const Vector2 &b) {
  return {a.x - b.x, a.y - b.y};
}

inline Vector2 operator-(const Vector2 &a) {
  return {-a.x, -a.y};
}

inline Vector2 operator-=(Vector2 &a, const Vector2 &b) {
  a = a - b;
  return a;
}

// Addition
inline Vector2 operator+(const Vector2 &a, const Vector2 &b) {
  return {a.x + b.x, a.y + b.y};
}

inline Vector2 operator+=(Vector2 &a, const Vector2 &b) {
  a = a + b;
  return a;
}

inline Vector3 operator+(const Vector3 &a, const Vector3 &b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline Vector3 operator+=(Vector3 &a, const Vector3 &b) {
  a = a + b;
  return a;
}

// Division
inline Vector2 operator/(const Vector2 &a, const float &b) {
  return {a.x / b, a.y / b};
}

// ELement-wise division
inline Vector2 operator/(const Vector2 &a, const Vector2 &b) {
  return {a.x / b.x, a.y / b.y};
}

inline Vector3 operator/(const Vector3 &a, const float &b) {
  return {a.x / b, a.y / b, a.z / b};
}


// Multiplication
inline Vector2 operator*(const Vector2 &a, const float &b) {
  return {a.x * b, a.y * b};
}

// Multiplication
inline Vector2 operator*(const float &a, const Vector2 &b) {
  return {a * b.x, a * b.y};
}

inline Vector2 operator*=(Vector2 &a, const float &b) {
  a = a * b;
  return a;
}

inline Vector3 operator*(const Vector3 &a, const float &b) {
  return {a.x * b, a.y * b, a.z * b};
}

inline Vector3 operator*=(Vector3 &a, const float &b) {
  a = a * b;
  return a;
}

// Elementwise multiplication
inline Vector2 operator*(const Vector2 &a, const Vector2 &b) {
  return {a.x * b.x, a.y * b.y};
}


// Dot product
inline float dot(const Vector2 &a, const Vector2 &b) {
  return a.x * b.x + a.y * b.y;
}

inline Vector2 lerp(const Vector2 &a, const Vector2 &b, const float &t) {
  return a * t + b * (1.0f - t);
}

// Returns the magnitude of the vector squared
inline float length_squared(const Vector2 &vec) { return vec.x * vec.x + vec.y * vec.y; }

// Returns the magnitude of the vector
inline float length(const Vector2 &vec) { return sqrt(length_squared(vec)); }

// Normalizes vector in place and returns it
inline Vector2 normalize(Vector2 &vec) {
  vec = vec / length(vec);

  return vec;
}

inline Vector2 limit_mag(const Vector2 &vec, float length) {
  float mag_sq = length_squared(vec);

  if (mag_sq > length * length) {
    return vec / sqrt(mag_sq) * length;
  } else {
    return vec;
  }
}

// returns a normalized version of the vector
inline Vector2 normalized(const Vector2 &vec) { 
  return vec / length(vec);
}

inline std::ostream &operator<<(std::ostream &os, const Vector2 &v) {
  os << "V2{ " << v.x << ", " << v.y << " }";
  return os;
}

inline float cross_product(const Vector2 &a, const Vector2 &b) {
  return a.x * b.y - a.y * b.x;
}

inline float manhatan_length(const Vector2 &vec) {
  return vec.x + vec.y;
}

// returns a vector rotated counter-clockwise by 90°
inline Vector2 normal(const Vector2 &vec) {
  return {-vec.y, vec.x};
}

///

// Intersect two lines
inline float intersect_lines(const Vector2 &o1, const Vector2 &v1, const Vector2 &o2, const Vector2 &v2) {
  Vector2 n = normal(v2);

  return dot((o2 - o1), n) / dot(v1, n);
}