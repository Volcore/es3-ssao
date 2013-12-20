/*******************************************************************************
    Copyright (c) 2012, Limbic Software, Inc.
    All rights reserved.
    This code is subject to the Google C++ Coding conventions:
         http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
 ******************************************************************************/
#ifndef LAMB_MATH_VECTOR2I_H_
#define LAMB_MATH_VECTOR2I_H_

#include "lamb/codingguides.h"

struct Vector2i {
  union {
    struct {
      int32_t x;
      int32_t y;
    };
    int32_t v[2];
  };
  explicit Vector2i() { }
  ~Vector2i() { }
  explicit inline Vector2i(int32_t x, int32_t y) {
    this->x = x;
    this->y = y;
  }
  explicit inline Vector2i(float v[2]) {
    this->v[0] = v[0];
    this->v[1] = v[1];
  }
  inline Vector2i(const Vector2i& a) {
    this->v[0] = a.v[0];
    this->v[1] = a.v[1];
  }
  inline void add(const Vector2i& a, const Vector2i& b) {
    x = a.x + b.x;
    y = a.y + b.y;
  }
  inline bool Equals(const Vector2i &a) {
    return a.x == x && a.y == y;
  }
};

#endif  // LAMB_MATH_VECTOR2I_H_
