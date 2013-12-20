/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#include "lamb/math/vector3f.h"
#include <cmath>
#include "lamb/math/vector2f.h"
#include "lamb/math/vector4f.h"
#include "lamb/math/matrix33f.h"

Vector3f::Vector3f() {
}

Vector3f::Vector3f(const float x, const float y, const float z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

Vector3f::Vector3f(const float v[3]) {
  this->x = v[0];
  this->y = v[1];
  this->z = v[2];
}

Vector3f::Vector3f(const Vector3f& a) {
  this->x = a.x;
  this->y = a.y;
  this->z = a.z;
}

Vector3f::Vector3f(const Vector4f& v) {
  x = v.x;
  y = v.y;
  z = v.z;
}

Vector3f::~Vector3f() {
}

float Vector3f::SquaredLength() const {
  return x*x+y*y+z*z;
}

float Vector3f::Length() const {
  float squared_length = SquaredLength();
  return sqrtf(squared_length);
}

void Vector3f::Scale(const float s) {
  x *= s;
  y *= s;
  z *= s;
}

void Vector3f::Normalize() {
  float length = Length();
  if (length <= 0.0f) {
    return;
  }
  float inv = 1.0f/length;
  Scale(inv);
}

void Vector3f::ScalePerComponent(const Vector3f& s) {
  x *= s.v[0];
  y *= s.v[1];
  z *= s.v[2];
}

void Vector3f::NormalizeFrom(const Vector3f& a) {
  float length = a.Length();
  if (length <= 0.0f) {
    return;
  }
  float inv = 1.0f/length;
  x = a.x*inv;
  y = a.y*inv;
  z = a.z*inv;
}

void Vector3f::Add(const Vector3f& a, const Vector3f& b) {
  x = a.x+b.x;
  y = a.y+b.y;
  z = a.z+b.z;
}

void Vector3f::Sub(const Vector3f& a, const Vector3f& b) {
  x = a.x-b.x;
  y = a.y-b.y;
  z = a.z-b.z;
}

void Vector3f::Cross(const Vector3f& a, const Vector3f& b) {
  x = a.y*b.z - a.z*b.y;
  y = a.z*b.x - a.x*b.z;
  z = a.x*b.y - a.y*b.x;
}

float Vector3f::Dot(const Vector3f& a, const Vector3f& b) {
  return a.x*b.x+a.y*b.y+a.z*b.z;
}

float Vector3f::Distance(const Vector3f& a, const Vector3f& b) {
  Vector3f c;
  c.Sub(b, a);
  return c.Length();
}

float Vector3f::DistanceSqr(const Vector3f& a, const Vector3f& b) {
  Vector3f c;
  c.Sub(b, a);
  return c.SquaredLength();
}

void Vector3f::ScaleAdd(const Vector3f& a, const float s, const Vector3f& b) {
  x = a.x+s*b.x;
  y = a.y+s*b.y;
  z = a.z+s*b.z;
}

void Vector3f::Lerp(const Vector3f& a, const float c, const Vector3f& b) {
  float inv = 1.0f-c;
  x = inv*a.x+c*b.x;
  y = inv*a.y+c*b.y;
  z = inv*a.z+c*b.z;
}

bool Vector3f::EpsilonEqual(const Vector3f& a, const float epsilon) const {
  Vector3f c;
  c.Sub(*this, a);
  if (fabsf(c.x) > epsilon)
    return false;
  if (fabsf(c.y) > epsilon)
    return false;
  if (fabsf(c.z) > epsilon)
    return false;
  return true;
}

void Vector3f::Ceil() {
  x = ceilf(x);
  y = ceilf(y);
  z = ceilf(z);
}

void Vector3f::Floor() {
  x = floorf(x);
  y = floorf(y);
  z = floorf(z);
}

void Vector3f::NormalizedDirectionFromTo(const Vector3f& a, const Vector3f& b) {
  Sub(b, a);
  Normalize();
}

Vector2f Vector3f::Dehomogenize() const {
  if (z == 0) {
    return Vector2f(0.0f, 0.0f);
  }
  float invz = 1.0f/z;
  return Vector2f(x * invz, y * invz);
}

void Vector3f::Print(const char *const name) const {
  if (name != 0) {
    Log("Vector3f \"%s\"\n", name);
  }
  Log("%f %f %f\n", x, y, z);
}

void Vector3f::Clamp(const Vector3f &min, const Vector3f &max) {
  x = clamp(min.x, x, max.x);
  y = clamp(min.y, y, max.y);
  z = clamp(min.z, z, max.z);
}

void Vector3f::Min(const Vector3f &a) {
  x = std::min(a.x, x);
  y = std::min(a.y, y);
  z = std::min(a.z, z);
}

void Vector3f::Max(const Vector3f &a) {
  x = std::max(a.x, x);
  y = std::max(a.y, y);
  z = std::max(a.z, z);
}

void Vector3f::ClosestPointOnLine(const Vector3f &a, const Vector3f &b, const Vector3f &p, bool do_clamp) {
  Vector3f v;
  v.Sub(p, a);
  Vector3f u;
  u.Sub(b, a);
  float l = u.Length();
  u.Scale(1.0f/l);
  float lambda = Vector3f::Dot(u, v);
  if (do_clamp) {
    lambda = clamp(0.0f, lambda, l);
  }
  ScaleAdd(a, lambda, u);
}

void Vector3f::Slerp(const Vector3f& a, const float c, const Vector3f& b, const Vector3f &fail_axis, float max_velocity) {
  // Compute the axis
  Vector3f axis;
  axis.Cross(a, b);
  float axis_sqr_length = axis.SquaredLength();
  float cos_theta = Vector3f::Dot(a, b);
  float angle = 0.0f;
  if (axis_sqr_length > 0.01f) {
    float length = sqrtf(axis_sqr_length);
    axis.Scale(1.0f/length);
    angle = atan2f(length, cos_theta);
  } else {
    if (cos_theta > 0.0f) {
      // Very close, use lerp
      Lerp(a, c, b);
      Normalize();
      return;
    } else {
      // use aux axis
      float length = sqrtf(axis_sqr_length);
      axis = fail_axis;
      angle = atan2f(length, cos_theta);
    }
  }
  angle *= c;
  if (max_velocity > 0.0f && fabsf(angle) > max_velocity) {
    angle = angle < 0 ? -max_velocity : max_velocity;
  }
  Matrix33f m(Matrix33f::Rotation(angle, axis));
  *this = m.Transform(a);
}

void Vector3f::FakeSlerp(const Vector3f& a, const float c, const Vector3f& b, float max_velocity) {
  float amount = c;
  if (max_velocity >= 0.0f) {
    // Find the angle
    float cos_theta = Vector3f::Dot(a, b);
    float theta = acosf(cos_theta);
    float abs_theta = fabsf(theta);
    if (abs_theta > max_velocity) {
      // Compute the fraction of rotation that is to be performed
      amount *= max_velocity / abs_theta;
    }
  }
  // Rotate by amount
  Lerp(a, amount, b);
  Normalize();
}

void Vector3f::MakeParallelToPlane(const Vector3f &normal) {
  float distance_from_plane = Dot(*this, normal);
  ScaleAdd(*this, -distance_from_plane, normal);
}

void Vector3f::ProjectToPlane(const Vector3f &normal, float distance) {
  float d = Dot(*this, normal);
  ScaleAdd(*this, distance - d, normal);
}

bool Vector3f::FindLineSegmentIntersection(const Vector3f &l0a, const Vector3f &l0b, const Vector3f &l1a, const Vector3f &l1b, Vector3f *p, float *l0t) {
  // returns true if line segments intersect within bounds
  Vector3f a, b, c;
  a.Sub(l0b, l0a);
  b.Sub(l1b, l1a);
  c.Sub(l1a, l0a);
  Vector3f axb;
  axb.Cross(a, b);
  float u = Dot(c, axb);
  if (fabsf(u) > 0.001f) {
    // Lines not coplanar
    return false;
  }
  Vector3f cxb;
  cxb.Cross(c, b);
  float s = Dot(cxb, axb) / axb.SquaredLength();
  if (s >= 0.0f && s <= 1.0f) {
    // parameter inside range
    *l0t = s;
    p->ScaleAdd(l0a, s, a);
    return true;
  }
  return false;
}

