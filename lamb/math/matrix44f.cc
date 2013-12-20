/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#include "lamb/math/matrix44f.h"
#include <stdio.h>
#include <algorithm>
#include "lamb/math/math.h"
#include "lamb/math/vector3f.h"
#include "lamb/math/vector4f.h"
#include "lamb/math/matrix33f.h"

Matrix44f::Matrix44f() {
}

Matrix44f::Matrix44f(float m00, float m01, float m02, float m03,
                     float m10, float m11, float m12, float m13,
                     float m20, float m21, float m22, float m23,
                     float m30, float m31, float m32, float m33)
    : m00(m00), m01(m01), m02(m02), m03(m03),
      m10(m10), m11(m11), m12(m12), m13(m13),
      m20(m20), m21(m21), m22(m22), m23(m23),
      m30(m30), m31(m31), m32(m32), m33(m33) {
}

Matrix44f::Matrix44f(const float a[16])
    : m00(a[0]), m01(a[4]), m02(a[8]), m03(a[12]),
      m10(a[1]), m11(a[5]), m12(a[9]), m13(a[13]),
      m20(a[2]), m21(a[6]), m22(a[10]), m23(a[14]),
      m30(a[3]), m31(a[7]), m32(a[11]), m33(a[15]) {
}

Matrix44f::Matrix44f(const Matrix44f &a)
    : m00(a.m[0]), m01(a.m[4]), m02(a.m[8]), m03(a.m[12]),
      m10(a.m[1]), m11(a.m[5]), m12(a.m[9]), m13(a.m[13]),
      m20(a.m[2]), m21(a.m[6]), m22(a.m[10]), m23(a.m[14]),
      m30(a.m[3]), m31(a.m[7]), m32(a.m[11]), m33(a.m[15]) {
}

Matrix44f::~Matrix44f() {
}

void Matrix44f::Transpose() {
  std::swap(m01, m10);
  std::swap(m02, m20);
  std::swap(m03, m30);
  std::swap(m12, m21);
  std::swap(m13, m31);
  std::swap(m23, m32);
}

void Matrix44f::TransposeFrom(const Matrix44f &a) {
  m00 = a.m00;
  m01 = a.m10;
  m02 = a.m20;
  m03 = a.m30;
  m10 = a.m01;
  m11 = a.m11;
  m12 = a.m21;
  m13 = a.m31;
  m20 = a.m02;
  m21 = a.m12;
  m22 = a.m22;
  m23 = a.m32;
  m30 = a.m03;
  m31 = a.m13;
  m32 = a.m23;
  m33 = a.m33;
}

void Matrix44f::TransposeInto(Matrix44f *a) const {
  a->m00 = m00;
  a->m01 = m10;
  a->m02 = m20;
  a->m03 = m30;
  a->m10 = m01;
  a->m11 = m11;
  a->m12 = m21;
  a->m13 = m31;
  a->m20 = m02;
  a->m21 = m12;
  a->m22 = m22;
  a->m23 = m32;
  a->m30 = m03;
  a->m31 = m13;
  a->m32 = m23;
  a->m33 = m33;
}

Matrix44f Matrix44f::Translation(const float x, const float y, const float z) {
  return Matrix44f(1.0f, 0.0f, 0.0f, x,
                   0.0f, 1.0f, 0.0f, y,
                   0.0f, 0.0f, 1.0f, z,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix44f Matrix44f::Translation(const Vector3f& v) {
  return Matrix44f(1.0f, 0.0f, 0.0f, v.x,
                   0.0f, 1.0f, 0.0f, v.y,
                   0.0f, 0.0f, 1.0f, v.z,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

void Matrix44f::Translate(const float x, const float y, const float z) {
  m03 += x;
  m13 += y;
  m23 += z;
}

void Matrix44f::Translate(const Vector3f& v) {
  m03 += v.x;
  m13 += v.y;
  m23 += v.z;
}

void Matrix44f::glTranslate(const float x, const float y, const float z) {
  Matrix44f translate(Matrix44f::Translation(x, y, z));
  Matrix44f tmp;
  tmp.Multiply(*this, translate);
  *this = tmp;
}

void Matrix44f::glTranslate(const Vector3f& v) {
  Matrix44f translate(Matrix44f::Translation(v.x, v.y, v.z));
  Matrix44f tmp;
  tmp.Multiply(*this, translate);
  *this = tmp;
}

void Matrix44f::glRotate(const float angle, const Vector3f& axis) {
  Matrix44f rotate(Matrix44f::Rotation(angle, axis));
  Matrix44f tmp;
  tmp.Multiply(*this, rotate);
  *this = tmp;
}

void Matrix44f::glScale(const float x, const float y, const float z) {
  Matrix44f scale(Matrix44f::Scaling(x, y, z));
  Matrix44f tmp;
  tmp.Multiply(*this, scale);
  *this = tmp;
}

void Matrix44f::glScale(const Vector3f& v) {
  Matrix44f scale(Matrix44f::Scaling(v.x, v.y, v.z));
  Matrix44f tmp;
  tmp.Multiply(*this, scale);
  *this = tmp;
}

Matrix44f Matrix44f::Scaling(const float x, const float y, const float z) {
  return Matrix44f(x, 0.0f, 0.0f, 0.0f,
                   0.0f, y, 0.0f, 0.0f,
                   0.0f, 0.0f, z, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix44f Matrix44f::Scaling(const Vector3f& s) {
  return Matrix44f(s.x, 0.0f, 0.0f, 0.0f,
                   0.0f, s.y, 0.0f, 0.0f,
                   0.0f, 0.0f, s.z, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

void Matrix44f::Scale(const float x, const float y, const float z) {
  Matrix44f scaling(Matrix44f::Scaling(x, y, z));
  Matrix44f tmp;
  tmp.Multiply(scaling, *this);
  *this = tmp;
}

void Matrix44f::Scale(const Vector3f& s) {
  Scale(s.x, s.y, s.z);
}

Matrix44f Matrix44f::Rotation(const float angle, const Vector3f& axis) {
  float x = axis.x;
  float y = axis.y;
  float z = axis.z;
  float s = sinf(angle);
  float c = cosf(angle);
  float oc = 1.0f-c;
  return Matrix44f(x*x*oc+c,   x*y*oc-z*s, x*z*oc+y*s, 0.0f,
                   y*x*oc+z*s, y*y*oc+c,   y*z*oc-x*s, 0.0f,
                   x*z*oc-y*s, y*z*oc+x*s, z*z*oc+c,   0.0f,
                   0.0f,       0.0f,       0.0f,       1.0f);
}

void Matrix44f::Rotate(const float angle, const Vector3f& axis) {
  Matrix44f rotation(Matrix44f::Rotation(angle, axis));
  Matrix44f tmp;
  tmp.Multiply(rotation, *this);
  *this = tmp;
}

void Matrix44f::InvertFrom(const Matrix44f& a) {
  a.InvertInto(this);
}

void Matrix44f::InvertInto(Matrix44f *a) const {
  float fA0 = m[ 0]*m[ 5] - m[ 1]*m[ 4];
  float fA1 = m[ 0]*m[ 6] - m[ 2]*m[ 4];
  float fA2 = m[ 0]*m[ 7] - m[ 3]*m[ 4];
  float fA3 = m[ 1]*m[ 6] - m[ 2]*m[ 5];
  float fA4 = m[ 1]*m[ 7] - m[ 3]*m[ 5];
  float fA5 = m[ 2]*m[ 7] - m[ 3]*m[ 6];
  float fB0 = m[ 8]*m[13] - m[ 9]*m[12];
  float fB1 = m[ 8]*m[14] - m[10]*m[12];
  float fB2 = m[ 8]*m[15] - m[11]*m[12];
  float fB3 = m[ 9]*m[14] - m[10]*m[13];
  float fB4 = m[ 9]*m[15] - m[11]*m[13];
  float fB5 = m[10]*m[15] - m[11]*m[14];
  float fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
  if (fabsf(fDet) < 0.0000001f) {
    return;
  }
  a->m[ 0] = m[ 5]*fB5 - m[ 6]*fB4 + m[ 7]*fB3;
  a->m[ 4] = - m[ 4]*fB5 + m[ 6]*fB2 - m[ 7]*fB1;
  a->m[ 8] = m[ 4]*fB4 - m[ 5]*fB2 + m[ 7]*fB0;
  a->m[12] = - m[ 4]*fB3 + m[ 5]*fB1 - m[ 6]*fB0;
  a->m[ 1] = - m[ 1]*fB5 + m[ 2]*fB4 - m[ 3]*fB3;
  a->m[ 5] = m[ 0]*fB5 - m[ 2]*fB2 + m[ 3]*fB1;
  a->m[ 9] = - m[ 0]*fB4 + m[ 1]*fB2 - m[ 3]*fB0;
  a->m[13] = m[ 0]*fB3 - m[ 1]*fB1 + m[ 2]*fB0;
  a->m[ 2] = m[13]*fA5 - m[14]*fA4 + m[15]*fA3;
  a->m[ 6] = - m[12]*fA5 + m[14]*fA2 - m[15]*fA1;
  a->m[10] = m[12]*fA4 - m[13]*fA2 + m[15]*fA0;
  a->m[14] = - m[12]*fA3 + m[13]*fA1 - m[14]*fA0;
  a->m[ 3] = - m[ 9]*fA5 + m[10]*fA4 - m[11]*fA3;
  a->m[ 7] = m[ 8]*fA5 - m[10]*fA2 + m[11]*fA1;
  a->m[11] = - m[ 8]*fA4 + m[ 9]*fA2 - m[11]*fA0;
  a->m[15] = m[ 8]*fA3 - m[ 9]*fA1 + m[10]*fA0;
  float fInvDet = 1.0f/fDet;
  a->m[ 0] *= fInvDet;
  a->m[ 1] *= fInvDet;
  a->m[ 2] *= fInvDet;
  a->m[ 3] *= fInvDet;
  a->m[ 4] *= fInvDet;
  a->m[ 5] *= fInvDet;
  a->m[ 6] *= fInvDet;
  a->m[ 7] *= fInvDet;
  a->m[ 8] *= fInvDet;
  a->m[ 9] *= fInvDet;
  a->m[10] *= fInvDet;
  a->m[11] *= fInvDet;
  a->m[12] *= fInvDet;
  a->m[13] *= fInvDet;
  a->m[14] *= fInvDet;
  a->m[15] *= fInvDet;
}

Matrix44f Matrix44f::Identity() {
  return Matrix44f(1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix44f Matrix44f::Zero() {
  return Matrix44f(0.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 0.0f);
}

Vector4f Matrix44f::Transform(const Vector4f& v) const {
  Vector4f r;
  r.x = m[0]*v.x+m[4]*v.y+m[ 8]*v.z+m[12]*v.w;
  r.y = m[1]*v.x+m[5]*v.y+m[ 9]*v.z+m[13]*v.w;
  r.z = m[2]*v.x+m[6]*v.y+m[10]*v.z+m[14]*v.w;
  r.w = m[3]*v.x+m[7]*v.y+m[11]*v.z+m[15]*v.w;
  return r;
}

Vector4f Matrix44f::TransformTransposed(const Vector4f& v) const {
  Vector4f r;
  r.x = m[0]*v.x+m[1]*v.y+m[2]*v.z+m[3]*v.w;
  r.y = m[4]*v.x+m[5]*v.y+m[6]*v.z+m[7]*v.w;
  r.z = m[8]*v.x+m[9]*v.y+m[10]*v.z+m[11]*v.w;
  r.w = m[12]*v.x+m[13]*v.y+m[14]*v.z+m[15]*v.w;
  return r;
}

Vector3f Matrix44f::TransformUpper33(const Vector3f& v) const {
  Vector3f r;
  r.x = m[0]*v.x+m[4]*v.y+m[8]*v.z;
  r.y = m[1]*v.x+m[5]*v.y+m[9]*v.z;
  r.z = m[2]*v.x+m[6]*v.y+m[10]*v.z;
  return r;
}

Vector3f Matrix44f::TransformTransposedUpper33(const Vector3f& v) const {
  Vector3f r;
  r.x = m[0]*v.x+m[1]*v.y+m[2]*v.z;
  r.y = m[4]*v.x+m[5]*v.y+m[6]*v.z;
  r.z = m[8]*v.x+m[9]*v.y+m[10]*v.z;
  return r;
}

void Matrix44f::Multiply(const Matrix44f& matrixLeft, const Matrix44f& matrixRight) {
  //  mij
  //  a00   a01   a02   [b00] b01 b02   m00  m01 m02
  // [a10] [a11] [a12]  [b10] b11 b12  [m10] m11 m12
  //  a20   a21   a22   [b20] b21 b22   m20  m21 m22
  m[0]  = matrixLeft.m[0] * matrixRight.m[0]  + matrixLeft.m[4] * matrixRight.m[1]  + matrixLeft.m[8] * matrixRight.m[2]   + matrixLeft.m[12] * matrixRight.m[3];
  m[4]  = matrixLeft.m[0] * matrixRight.m[4]  + matrixLeft.m[4] * matrixRight.m[5]  + matrixLeft.m[8] * matrixRight.m[6]   + matrixLeft.m[12] * matrixRight.m[7];
  m[8]  = matrixLeft.m[0] * matrixRight.m[8]  + matrixLeft.m[4] * matrixRight.m[9]  + matrixLeft.m[8] * matrixRight.m[10]  + matrixLeft.m[12] * matrixRight.m[11];
  m[12] = matrixLeft.m[0] * matrixRight.m[12] + matrixLeft.m[4] * matrixRight.m[13] + matrixLeft.m[8] * matrixRight.m[14]  + matrixLeft.m[12] * matrixRight.m[15];

  m[1]  = matrixLeft.m[1] * matrixRight.m[0]  + matrixLeft.m[5] * matrixRight.m[1]  + matrixLeft.m[9] * matrixRight.m[2]   + matrixLeft.m[13] * matrixRight.m[3];
  m[5]  = matrixLeft.m[1] * matrixRight.m[4]  + matrixLeft.m[5] * matrixRight.m[5]  + matrixLeft.m[9] * matrixRight.m[6]   + matrixLeft.m[13] * matrixRight.m[7];
  m[9]  = matrixLeft.m[1] * matrixRight.m[8]  + matrixLeft.m[5] * matrixRight.m[9]  + matrixLeft.m[9] * matrixRight.m[10]  + matrixLeft.m[13] * matrixRight.m[11];
  m[13] = matrixLeft.m[1] * matrixRight.m[12] + matrixLeft.m[5] * matrixRight.m[13] + matrixLeft.m[9] * matrixRight.m[14]  + matrixLeft.m[13] * matrixRight.m[15];

  m[2]  = matrixLeft.m[2] * matrixRight.m[0]  + matrixLeft.m[6] * matrixRight.m[1]  + matrixLeft.m[10] * matrixRight.m[2]  + matrixLeft.m[14] * matrixRight.m[3];
  m[6]  = matrixLeft.m[2] * matrixRight.m[4]  + matrixLeft.m[6] * matrixRight.m[5]  + matrixLeft.m[10] * matrixRight.m[6]  + matrixLeft.m[14] * matrixRight.m[7];
  m[10] = matrixLeft.m[2] * matrixRight.m[8]  + matrixLeft.m[6] * matrixRight.m[9]  + matrixLeft.m[10] * matrixRight.m[10] + matrixLeft.m[14] * matrixRight.m[11];
  m[14] = matrixLeft.m[2] * matrixRight.m[12] + matrixLeft.m[6] * matrixRight.m[13] + matrixLeft.m[10] * matrixRight.m[14] + matrixLeft.m[14] * matrixRight.m[15];

  m[3]  = matrixLeft.m[3] * matrixRight.m[0]  + matrixLeft.m[7] * matrixRight.m[1]  + matrixLeft.m[11] * matrixRight.m[2]  + matrixLeft.m[15] * matrixRight.m[3];
  m[7]  = matrixLeft.m[3] * matrixRight.m[4]  + matrixLeft.m[7] * matrixRight.m[5]  + matrixLeft.m[11] * matrixRight.m[6]  + matrixLeft.m[15] * matrixRight.m[7];
  m[11] = matrixLeft.m[3] * matrixRight.m[8]  + matrixLeft.m[7] * matrixRight.m[9]  + matrixLeft.m[11] * matrixRight.m[10] + matrixLeft.m[15] * matrixRight.m[11];
  m[15] = matrixLeft.m[3] * matrixRight.m[12] + matrixLeft.m[7] * matrixRight.m[13] + matrixLeft.m[11] * matrixRight.m[14] + matrixLeft.m[15] * matrixRight.m[15];
}

bool Matrix44f::EpsilonEqual(const Matrix44f& a, const float epsilon) {
  for (int i = 0; i < 16; ++i) {
    float diff = a.m[i]-m[i];
    if (fabsf(diff) >= epsilon) {
      return false;
    }
  }
  return true;
}

void Matrix44f::glOrthof(float l, float r, float b, float t, float n, float f) {
  Matrix44f ortho;
  float *p = ortho.m;
  float tx  = -(r+l)/(r-l);
  float ty  = -(t+b)/(t-b);
  float tz  = -(f+n)/(f-n);
  float x = 2.0f/(r-l);
  float y = 2.0f/(t-b);
  //  c  s 0 0     x 0 0 tx      c*x  s*y 0  c*tx+s*ty
  // -s  c 0 0  \/ 0 y 0 ty  -- -s*x  c*y 0 -s*tx+c*ty
  //  0  0 1 0  /\ 0 0 z tz  --   0    0  z     tz
  //  0  0 0 1     0 0 0 1        0    0  0      1
  // left multiplied we find a rotation around the z axis about roll radians
  p[0] = x;           p[ 4] = 0.0f;          p[ 8] = 0.0f;            p[12] = tx;    // NOLINT
  p[1] = 0.0f;        p[ 5] = y;             p[ 9] = 0.0f;            p[13] = ty;    // NOLINT
  p[2] = 0.0f;        p[ 6] = 0.0f;          p[10] = -2.0f/(f-n);     p[14] = tz;    // NOLINT
  p[3] = 0.0f;        p[ 7] = 0.0f;          p[11] = 0.0f;            p[15] = 1.0f;  // NOLINT
  Matrix44f tmp;
  tmp.Multiply(*this, ortho);
  *this = tmp;
}

void Matrix44f::Print(const char *const name) const {
  // so in the order
  //   0   4   8  12
  //   1   5   9  13
  //   2   6  10  14
  //   3   7  11  15
  if (name) {
    Log("Matrix \"%s\"\n", name);
  }
  Log("%f, %f, %f, %f\n", m[0], m[4], m[8], m[12]);
  Log("%f, %f, %f, %f\n", m[1], m[5], m[9], m[13]);
  Log("%f, %f, %f, %f\n", m[2], m[6], m[10], m[14]);
  Log("%f, %f, %f, %f\n", m[3], m[7], m[11], m[15]);
}

void Matrix44f::Upper3x3(Matrix33f *o) const {
  o->m00 = m00;
  o->m10 = m10;
  o->m20 = m20;
  o->m01 = m01;
  o->m11 = m11;
  o->m21 = m21;
  o->m02 = m02;
  o->m12 = m12;
  o->m22 = m22;
}

