/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_MATH_VECTOR4F_H_
#define LAMB_MATH_VECTOR4F_H_

#include "lamb/codingguides.h"

struct Vector3f;

struct Vector4f {
  union {
    struct {
      float x;
      float y;
      float z;
      float w;
    };
    float v[4];
  };
  // Leaves all members uninitialized
  explicit Vector4f();
  explicit Vector4f(const float x, const float y, const float z, const float w);
  explicit Vector4f(const float v[4]);
  explicit Vector4f(const Vector3f &v, float w);
  Vector4f(const Vector4f& a);
  ~Vector4f();
  float SquaredLength() const;
  // Computes the squared length only of the first 3 components
  float SquaredLengthPlane() const;
  float Length() const;
  // Computes the length only of the first 3 components
  float LengthPlane() const;
  void Scale(const float s);
  void Normalize();
  // Normalizes using only the length of the first 3 components
  void NormalizePlane();
  void ScalePerComponent(const Vector4f& s);
  void NormalizeFrom(const Vector4f& a);
  // Normalizes using only the length of the first 3 components
  void NormalizePlaneFrom(const Vector4f& a);
  void Add(const Vector4f& a, const Vector4f& b);
  void Sub(const Vector4f& a, const Vector4f& b);
  static float Dot(const Vector4f& a, const Vector4f& b);
  static float Distance(const Vector4f& a, const Vector4f& b);
  void ScaleAdd(const Vector4f& a, const float s, const Vector4f& b);
  void Lerp(const Vector4f& a, const float c, const Vector4f& b);
  bool EpsilonEqual(const Vector4f& a, const float epsilon) const;
  bool IsNan() const;
  inline Vector4f &AlphaModulate(float alpha) {
    w *= alpha;
    return *this;
  }
  Vector3f xyz() const;
  void Print(const char *const name = 0) const;
};

#endif  // LAMB_MATH_VECTOR4F_H_
