/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#include "lamb/math/vector4f.h"
#include <stdio.h>
#include <cmath>
#include "lamb/math/vector3f.h"

Vector4f::Vector4f() {
}

Vector4f::Vector4f(const float x, const float y, const float z, const float w) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
}

Vector4f::Vector4f(const float v[4]) {
  this->x = v[0];
  this->y = v[1];
  this->z = v[2];
  this->w = v[3];
}

Vector4f::Vector4f(const Vector4f& a) {
  this->x = a.x;
  this->y = a.y;
  this->z = a.z;
  this->w = a.w;
}

Vector4f::Vector4f(const Vector3f &v, float w) {
  this->x = v.x;
  this->y = v.y;
  this->z = v.z;
  this->w = w;
}

Vector4f::~Vector4f() {
}

float Vector4f::SquaredLength() const {
  return x*x+y*y+z*z+w*w;
}

float Vector4f::SquaredLengthPlane() const {
  return x*x+y*y+z*z;
}

float Vector4f::Length() const {
  float squared_length = SquaredLength();
  return sqrtf(squared_length);
}

float Vector4f::LengthPlane() const {
  float squared_length = SquaredLengthPlane();
  return sqrtf(squared_length);
}

void Vector4f::Scale(const float s) {
  x *= s;
  y *= s;
  z *= s;
  w *= s;
}

void Vector4f::Normalize() {
  float length = Length();
  if (length <= 0.0f) {
    return;
  }
  float inv = 1.0f/length;
  Scale(inv);
}

void Vector4f::NormalizePlane() {
  float length = LengthPlane();
  if (length <= 0.0f) {
    return;
  }
  float inv = 1.0f/length;
  Scale(inv);
}

void Vector4f::ScalePerComponent(const Vector4f& s) {
  x *= s.v[0];
  y *= s.v[1];
  z *= s.v[2];
  w *= s.v[3];
}

void Vector4f::NormalizeFrom(const Vector4f& a) {
  float length = a.Length();
  if (length <= 0.0f) {
    return;
  }
  float inv = 1.0f/length;
  x = a.x*inv;
  y = a.y*inv;
  z = a.z*inv;
  w = a.w*inv;
}

void Vector4f::NormalizePlaneFrom(const Vector4f& a) {
  float length = a.LengthPlane();
  if (length <= 0.0f) {
    return;
  }
  float inv = 1.0f/length;
  x = a.x*inv;
  y = a.y*inv;
  z = a.z*inv;
  w = a.w*inv;
}

void Vector4f::Add(const Vector4f& a, const Vector4f& b) {
  x = a.x+b.x;
  y = a.y+b.y;
  z = a.z+b.z;
  w = a.w+b.w;
}

void Vector4f::Sub(const Vector4f& a, const Vector4f& b) {
  x = a.x-b.x;
  y = a.y-b.y;
  z = a.z-b.z;
  w = a.w-b.w;
}

float Vector4f::Dot(const Vector4f& a, const Vector4f& b) {
  return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w;
}

float Vector4f::Distance(const Vector4f& a, const Vector4f& b) {
  Vector4f c;
  c.Sub(b, a);
  return c.Length();
}

void Vector4f::ScaleAdd(const Vector4f& a, const float s, const Vector4f& b) {
  x = a.x+s*b.x;
  y = a.y+s*b.y;
  z = a.z+s*b.z;
  w = a.w+s*b.w;
}

void Vector4f::Lerp(const Vector4f& a, const float c, const Vector4f& b) {
  float inv = 1.0f-c;
  x = inv*a.x+c*b.x;
  y = inv*a.y+c*b.y;
  z = inv*a.z+c*b.z;
  w = inv*a.w+c*b.w;
}

bool Vector4f::EpsilonEqual(const Vector4f& a, const float epsilon) const {
  Vector4f c;
  c.Sub(*this, a);
  if (fabsf(c.x) > epsilon)
    return false;
  if (fabsf(c.y) > epsilon)
    return false;
  if (fabsf(c.z) > epsilon)
    return false;
  if (fabsf(c.w) > epsilon)
    return false;
  return true;
}

bool Vector4f::IsNan() const {
  return x != x || y != y || z != z || w != w;
}

Vector3f Vector4f::xyz() const {
  return Vector3f(x, y, z);
}

void Vector4f::Print(const char *const name) const {
  if (name != 0) {
    Log("Vector4f \"%s\"\n", name);
  }
  Log("%f %f %f %f\n", x, y, z, w);
}
