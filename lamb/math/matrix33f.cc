/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#include "lamb/math/matrix33f.h"
#include <stdio.h>
#include <algorithm>
#include "lamb/math/math.h"
#include "lamb/math/vector2f.h"
#include "lamb/math/vector3f.h"
#include "lamb/math/matrix44f.h"

Matrix33f::Matrix33f() {
}

Matrix33f::Matrix33f(float m00, float m01, float m02,
                     float m10, float m11, float m12,
                     float m20, float m21, float m22)
    : m00(m00), m01(m01), m02(m02),
      m10(m10), m11(m11), m12(m12),
      m20(m20), m21(m21), m22(m22) {
}

Matrix33f::Matrix33f(const float a[9])
    : m00(a[0]), m01(a[3]), m02(a[6]),
      m10(a[1]), m11(a[4]), m12(a[7]),
      m20(a[2]), m21(a[5]), m22(a[8]) {
}

Matrix33f::Matrix33f(const Matrix33f &a)
    : m00(a.m[0]), m01(a.m[3]), m02(a.m[6]),
      m10(a.m[1]), m11(a.m[4]), m12(a.m[7]),
      m20(a.m[2]), m21(a.m[5]), m22(a.m[8]) {
}

Matrix33f::~Matrix33f() {
}

void Matrix33f::Transpose() {
  std::swap(m01, m10);
  std::swap(m02, m20);
  std::swap(m12, m21);
}

void Matrix33f::TransposeFrom(const Matrix33f &a) {
  m00 = a.m00;
  m01 = a.m10;
  m02 = a.m20;
  m10 = a.m01;
  m11 = a.m11;
  m12 = a.m21;
  m20 = a.m02;
  m21 = a.m12;
  m22 = a.m22;
}

void Matrix33f::TransposeInto(Matrix33f *a) const {
  a->m00 = m00;
  a->m01 = m10;
  a->m02 = m20;
  a->m10 = m01;
  a->m11 = m11;
  a->m12 = m21;
  a->m20 = m02;
  a->m21 = m12;
  a->m22 = m22;
}

void Matrix33f::InvertFrom(const Matrix33f& a) {
  a.InvertInto(this);
}

void Matrix33f::InvertInto(Matrix33f *a) const {
  float fDet = m00 * (m22*m11-m21*m12)
             - m10 * (m22*m01-m21*m02)
             + m20 * (m12*m01-m11*m02);
  if (fabsf(fDet) < 0.00001f) {
    return;
  }
  a->m00 = m22*m11 - m21*m12;
  a->m10 = -(m22*m10 - m20*m12);
  a->m20 = m21*m10 - m20*m11;
  a->m01 = -(m22*m01 - m21*m02);
  a->m11 = m22*m00 - m20*m02;
  a->m21 = -(m21*m00 - m20*m01);
  a->m02 = m12*m01 - m11*m02;
  a->m12 = -(m12*m00 - m10*m02);
  a->m22 = m11*m00 - m10*m01;
  float fInvDet = 1.0f/fDet;
  for (int i = 0; i < 9; ++i) {
    a->m[i] *= fInvDet;
  }
}

float Matrix33f::Determinant() const {
  return m00 * m11 * m22
       + m10 * m21 * m02
       + m20 * m01 * m12
       - m20 * m11 * m02
       - m10 * m01 * m22
       - m00 * m21 * m12;
}

Matrix33f Matrix33f::Identity() {
  return Matrix33f(1.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 1.0f);
}

Vector3f Matrix33f::Transform(const Vector3f& v) const {
  Vector3f r;
  r.x = m[0]*v.x+m[3]*v.y+m[6]*v.z;
  r.y = m[1]*v.x+m[4]*v.y+m[7]*v.z;
  r.z = m[2]*v.x+m[5]*v.y+m[8]*v.z;
  return r;
}

Vector3f Matrix33f::TransformTransposed(const Vector3f& v) const {
  Vector3f r;
  r.x = m[0]*v.x+m[1]*v.y+m[2]*v.z;
  r.y = m[3]*v.x+m[4]*v.y+m[5]*v.z;
  r.z = m[6]*v.x+m[7]*v.y+m[8]*v.z;
  return r;
}

void Matrix33f::Multiply(const Matrix33f& a, const Matrix33f& b) {
  int i, j, k;
  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 3; ++j) {
      m[i+j*3] = 0.0f;
      for (k = 0; k < 3; ++k) {
        m[i+j*3] += a.m[i+k*3]*b.m[k+j*3];
      }
    }
  }
}

bool Matrix33f::EpsilonEqual(const Matrix33f& a, const float epsilon) {
  for (int i = 0; i < 9; ++i) {
    float diff = a.m[i]-m[i];
    if (fabsf(diff) >= epsilon) {
      return false;
    }
  }
  return true;
}

void Matrix33f::Scale(float s) {
  for (int i = 0; i < 9; ++i) {
    m[i] *= s;
  }
}

void Matrix33f::Print(const char *const name) const {
  // so in the order
  //   0   3   6
  //   1   4   7
  //   2   5   8
  if (name) {
    Log("Matrix \"%s\"\n", name);
  }
  Log("%f, %f, %f\n", m[0], m[3], m[6]);
  Log("%f, %f, %f\n", m[1], m[4], m[7]);
  Log("%f, %f, %f\n", m[2], m[5], m[8]);
}

void MakeNormalMatrix(Matrix33f *dst, const Matrix44f & src) {
  // Copy transposed upper 3x3
  Matrix33f tmp;
  tmp.m00 = src.m00;
  tmp.m10 = src.m01;
  tmp.m20 = src.m02;
  tmp.m01 = src.m10;
  tmp.m11 = src.m11;
  tmp.m21 = src.m12;
  tmp.m02 = src.m20;
  tmp.m12 = src.m21;
  tmp.m22 = src.m22;
  // Invert
  dst->InvertFrom(tmp);
}

Vector2f Matrix33f::Project(const Vector2f& p) const {
  float d = p.x * m20 + p.y * m21 + m22;
  float invd = 1.0f/d;
  float x = (p.x * m00 + p.y * m01 + m02) * invd;
  float y = (p.x * m10 + p.y * m11 + m12) * invd;
  return Vector2f(x, y);
}

void Matrix33f::CrossProduct(const Vector3f &a) {
  m00 = 0.0f;
  m10 = a.z;
  m20 = -a.y;
  m01 = -a.z;
  m11 = 0.0f;
  m21 = a.x;
  m02 = a.y;
  m12 = -a.x;
  m22 = 0.0f;
}

void Matrix33f::OuterProduct(const Vector3f &a, const Vector3f &bt) {
  m00 = a.x * bt.x;
  m01 = a.x * bt.y;
  m02 = a.x * bt.z;
  m10 = a.y * bt.x;
  m11 = a.y * bt.y;
  m12 = a.y * bt.z;
  m20 = a.z * bt.x;
  m21 = a.z * bt.y;
  m22 = a.z * bt.z;
}

void Matrix33f::Add(const Matrix33f &a, const Matrix33f &b) {
  for (int i = 0; i < 9; ++i) {
    m[i] = a.m[i] + b.m[i];
  }
}

float Matrix33f::RMSE(const Matrix33f &a) const {
  float error = 0.0f;
  for (int i = 0; i < 9; ++i) {
    error += sqr(m[i] - a.m[i]);
  }
  return sqrtf(error/9.0f);
}

Matrix33f Matrix33f::Rotation(const float angle, const Vector3f& axis) {
  float x = axis.x;
  float y = axis.y;
  float z = axis.z;
  float s = sinf(angle);
  float c = cosf(angle);
  float oc = 1.0f-c;
  return Matrix33f(x*x*oc+c,   x*y*oc-z*s, x*z*oc+y*s,
                   y*x*oc+z*s, y*y*oc+c,   y*z*oc-x*s,
                   x*z*oc-y*s, y*z*oc+x*s, z*z*oc+c);
}
