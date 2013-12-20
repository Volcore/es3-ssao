/*******************************************************************************
    Copyright (c) 2012, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#include "lamb/math/matrix22f.h"
#include "lamb/math/vector2f.h"

Matrix22f::Matrix22f() {
}

Matrix22f::Matrix22f(float m00, float m01, float m10, float m11)
    : m00(m00), m01(m01),
      m10(m10), m11(m11) {
}

Matrix22f::Matrix22f(const float a[4])
    : m00(a[0]), m01(a[2]),
      m10(a[1]), m11(a[3]) {
}

Matrix22f::Matrix22f(const Matrix22f &a)
    : m00(a.m[0]), m01(a.m[2]),
      m10(a.m[1]), m11(a.m[3]) {
}

Matrix22f::~Matrix22f() {
}

Matrix22f Matrix22f::Identity() {
  return Matrix22f(1.0f, 0.0f, 0.0f, 1.0f);
}

void Matrix22f::Scale(float s) {
  m00 *= s;
  m10 *= s;
  m01 *= s;
  m11 *= s;
}

void Matrix22f::InvertInto(Matrix22f *a) const {
  float det = m00*m11-m01*m10;
  if (det == 0.0f) {
    *a = Identity();
    return;
  }
  float norm = 1.0f/(det);
  a->m00 = m11*norm;
  a->m11 = m00*norm;
  a->m10 = -m10*norm;
  a->m01 = -m01*norm;
}

void Matrix22f::InvertFrom(const Matrix22f& a) {
  a.InvertInto(this);
}

void Matrix22f::Multiply(const Matrix22f& a, const Matrix22f& b) {
  int i, j, k;
  for (i = 0; i < 2; ++i) {
    for (j = 0; j < 2; ++j) {
      m[i+j*2] = 0.0f;
      for (k = 0; k < 2; ++k) {
        m[i+j*2] += a.m[i+k*2]*b.m[k+j*2];
      }
    }
  }
}

Vector2f Matrix22f::Transform(const Vector2f& v) const {
  Vector2f r;
  r.x = m[0]*v.x+m[2]*v.y;
  r.y = m[1]*v.x+m[3]*v.y;
  return r;
}
