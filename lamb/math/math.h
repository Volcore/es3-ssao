/*******************************************************************************
    Copyright (c) 2009, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_MATH_MATH_H_
#define LAMB_MATH_MATH_H_

#include <stdint.h>
#include <cmath>
#include <algorithm>
#include "lamb/math/color.h"

static const float F_PI = 3.1415925026f;
static const float SQRT2 = 1.414213562f;
static inline float deg2Rad(const float& x) { return x*F_PI/180.0f; }
static inline float rad2Deg(const float& x) { return x*180.0f/F_PI; }
static inline float ft_to_m(const float& x) { return x*0.3048f; }
static inline float m_to_ft(const float& x) { return x*3.2808399f; }
static inline float closeToZero(const float& x, const float epsilon = 0.0001f) { return (x < epsilon && x > -epsilon); }

static inline int IncreaseToEven(int x) { return (x & 1) == 0 ? x : x + 1; }

template<typename T> static inline T pow3(const T& x) { return x*x*x; }

template<typename T> static inline T max(const T& a, const T& b) {
  return (a>b)?a:b;
}
template<typename T> static inline T min(const T& a, const T& b) {
  return (a < b)?a:b;
}

static inline bool IsNan(float x) {
  return x != x;
}

template<typename T> static inline T clamp(const T& a, const T& c, const T& b) {
  return max(a, min(c, b));
}

static inline float lerp(const float a, const float lambda, const float b) {
  return a + (b - a)*lambda;
}
template<typename T> static inline T sqr(const T& x) {
  return x*x;
}

template<typename T> static inline T animate_value(const T& target, const T& current, const T& rate) {
  return current + clamp(-rate, target - current, rate);
}

static inline int fast_floorf_to_int(const float &x) {
  return x >= 0?static_cast<int>(x):static_cast<int>(x)-1;
}

static float EaseIn2(float t) { return sqr(t); }
static float EaseOut2(float t) { return 1-sqr(1-t); }
static float EaseInOut2(float t) {
  if (t < 0.5f) {
    return 0.5f*sqr(2.0f*t);
  } else {
    return 1.0f-0.5f*sqr(2.0f*t-2.0f);
  }
}
static float EaseIn3(float t) { return pow3(t); }
static float EaseOut3(float t) { return 1-pow3(1-t); }
static float EaseInOut3(float t) {
  if (t < 0.5f) {
    return 0.5f*pow3(2.0f*t);
  } else {
    return 1.0f+0.5f*pow3(2.0f*t-2.0f);
  }
}

static inline int positive_mod(int x, int m) { return ((x%m)+m)%m; }

static inline uint32_t fast_log2_32(uint32_t v) {
  static const uint32_t b[] = {
    0xAAAAAAAA,
    0xCCCCCCCC,
    0xF0F0F0F0,
    0xFF00FF00,
    0xFFFF0000
  };
  uint32_t r = (v & b[0]) != 0;
  for (int i = 4; i > 0; i--) {
    r |= ((v & b[i]) != 0) << i;
  }
  return r;
}

static inline uint16_t next_power_of_two16(uint16_t x) {
  --x;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  ++x;
  return x;
}

static inline float cscf(float x) {
  return - (2.0f*sinf(x))/(cosf(2.0f*x)-1.0f);
}

#endif  // LAMB_MATH_MATH_H_
