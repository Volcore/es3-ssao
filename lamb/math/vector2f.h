/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_MATH_VECTOR2F_H_
#define LAMB_MATH_VECTOR2F_H_

#include "lamb/codingguides.h"
#include "lamb/math/math.h"

struct Vector2f {
  union {
    struct {
      float x;
      float y;
    };
    float v[2];
  };
  explicit inline Vector2f() { }
  explicit inline Vector2f(float x, float y) {
    this->x = x;
    this->y = y;
  }
  explicit inline Vector2f(float v[2]) {
    this->v[0] = v[0];
    this->v[1] = v[1];
  }
  inline Vector2f(const Vector2f& a) {
    this->v[0] = a.v[0];
    this->v[1] = a.v[1];
  }
  inline ~Vector2f() { }
  inline void Add(const Vector2f& a, const Vector2f& b) {
    x = a.x + b.x;
    y = a.y + b.y;
  }
  inline void Scale(float s) {
    x *= s;
    y *= s;
  }
  inline void ScaleAdd(const Vector2f& a, float s, const Vector2f& b) {
    x = a.x + s * b.x;
    y = a.y + s * b.y;
  }
  inline void Sub(const Vector2f& a, const Vector2f& b) {
    x = a.x - b.x;
    y = a.y - b.y;
  }
  inline float SquaredLength() const {
    return x*x+y*y;
  }
  inline float Length() const {
    return sqrtf(SquaredLength());
  }
  static inline float dot(const Vector2f& a, const Vector2f& b) {
    return a.x*b.x+a.y*b.y;
  }
  static inline float DistanceSquared(const Vector2f &a, const Vector2f &b) {
    return sqr(a.x-b.x) + sqr(a.y-b.y);
  }
  inline void Normalize() {
    Scale(1.0f/Length());
  }
  void Lerp(const Vector2f& a, const float c, const Vector2f& b) {
    float inv = 1.0f - c;
    x = inv * a.x + c * b.x;
    y = inv * a.y + c * b.y;
  }
  void Print(const char *const name = 0) const;
  static inline float Dot(const Vector2f &a, const Vector2f &b) {
    return a.x*b.x + a.y*b.y;
  }
};

#endif  // LAMB_MATH_VECTOR2F_H_
