/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_MATH_VECTOR3F_H_
#define LAMB_MATH_VECTOR3F_H_

#include "lamb/codingguides.h"

struct Vector2f;
struct Vector4f;

struct Vector3f {
  union {
    struct {
      float x;
      float y;
      float z;
    };
    float v[3];
  };
  // Leaves all members uninitialized
  explicit Vector3f();
  explicit Vector3f(const float x, const float y, const float z);
  explicit Vector3f(const float v[3]);
  explicit Vector3f(const Vector4f& v);
  // The only non-explicit constructor is the copy constructor
  Vector3f(const Vector3f& a);
  ~Vector3f();
  float SquaredLength() const;
  float Length() const;
  void Scale(const float s);
  void Normalize();
  void ScalePerComponent(const Vector3f& s);
  void NormalizeFrom(const Vector3f& a);
  void NormalizedDirectionFromTo(const Vector3f& a, const Vector3f& b);
  void Add(const Vector3f& a, const Vector3f& b);
  void Sub(const Vector3f& a, const Vector3f& b);
  void Cross(const Vector3f& a, const Vector3f& b);
  void ClosestPointOnLine(const Vector3f &a, const Vector3f &b, const Vector3f &p, bool clamp = true);
  void MakeParallelToPlane(const Vector3f &normal);
  void ProjectToPlane(const Vector3f &normal, float distance);
  static bool FindLineSegmentIntersection(const Vector3f &l0a, const Vector3f &l0b, const Vector3f &l1a, const Vector3f &l1b, Vector3f *p, float *l0t);
  void Ceil();
  void Floor();
  void Print(const char *const name = 0) const;
  static float Dot(const Vector3f& a, const Vector3f& b);
  static float Distance(const Vector3f& a, const Vector3f& b);
  static float DistanceSqr(const Vector3f& a, const Vector3f& b);
  void Min(const Vector3f &a);
  void Max(const Vector3f &a);
  void ScaleAdd(const Vector3f& a, const float s, const Vector3f& b);
  void Lerp(const Vector3f& a, const float c, const Vector3f& b);
  void Slerp(const Vector3f& a, const float c, const Vector3f& b, const Vector3f &fail_axis, float max_velocity=-1.0f);
  void FakeSlerp(const Vector3f& a, const float c, const Vector3f& b, float max_velocity=-1.0f);
  bool EpsilonEqual(const Vector3f& a, const float epsilon) const;
  void Clamp(const Vector3f &min, const Vector3f &max);
  Vector2f Dehomogenize() const;
};

#endif  // LAMB_MATH_VECTOR3F_H_
