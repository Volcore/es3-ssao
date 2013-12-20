/*******************************************************************************
    Copyright (c) 2012, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_MATH_MATRIX22F_H_
#define LAMB_MATH_MATRIX22F_H_

#include "lamb/codingguides.h"

struct Vector2f;

struct Matrix22f {
 public:
  // Human Readable Matrix
  //  m00 m01
  //  m10 m11
  // is stored in memory as
  //  m00 m10 m01 m11
  // so in the order
  //   0   3
  //   1   4
  union {
    struct {
      float m00, m10;
      float m01, m11;
    };
    float m[4];
  };
  explicit Matrix22f();
  explicit Matrix22f(float m00, float m01,
                     float m10, float m11);
  explicit Matrix22f(const float a[4]);
  Matrix22f(const Matrix22f &a);
  ~Matrix22f();
  static Matrix22f Identity();
  void Scale(float s);
  void InvertFrom(const Matrix22f& a);
  void InvertInto(Matrix22f *a) const;
  void Multiply(const Matrix22f& a, const Matrix22f& b);
  Vector2f Transform(const Vector2f& vector) const;
};

#endif  // LAMB_MATH_MATRIX22F_H_
