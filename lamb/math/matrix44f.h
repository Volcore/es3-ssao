/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_MATH_MATRIX44F_H_
#define LAMB_MATH_MATRIX44F_H_

#include "lamb/codingguides.h"

struct Vector3f;
struct Vector4f;

struct Matrix44f {
  // Human Readable Matrix
  //  m00 m01 m02 m03
  //  m10 m11 m12 m13
  //  m20 m21 m22 m23
  //  m30 m31 m32 m33
  // is stored in memory as
  //  m00 m10 m20 m30 m01 m11 m21 m31 ...
  // so in the order
  //   0   4   8  12
  //   1   5   9  13
  //   2   6  10  14
  //   3   7  11  15
  union {
    struct {
      float m00, m10, m20, m30;
      float m01, m11, m21, m31;
      float m02, m12, m22, m32;
      float m03, m13, m23, m33;
    };
    float m[16];
  };
  explicit Matrix44f();
  explicit Matrix44f(float m00, float m01, float m02, float m03,
                     float m10, float m11, float m12, float m13,
                     float m20, float m21, float m22, float m23,
                     float m30, float m31, float m32, float m33);
  explicit Matrix44f(const float a[16]);
  // The only non-explicit constructor is the copy constructor
  Matrix44f(const Matrix44f &a);
  ~Matrix44f();
  void Transpose();
  void TransposeFrom(const Matrix44f &a);
  void TransposeInto(Matrix44f *a) const;
  static Matrix44f Translation(const float x, const float y, const float z);
  static Matrix44f Translation(const Vector3f& v);
  void Translate(const float x, const float y, const float z);
  void Translate(const Vector3f& v);
  void glTranslate(const float x, const float y, const float z);
  void glTranslate(const Vector3f& v);
  static Matrix44f Scaling(const float x, const float y, const float z);
  static Matrix44f Scaling(const Vector3f& s);
  void Scale(const float x, const float y, const float z);
  void Scale(const Vector3f& s);
  void glScale(const float x, const float y, const float z);
  void glScale(const Vector3f& s);
  static Matrix44f Rotation(const float angle, const Vector3f& axis);
  void Rotate(const float angle, const Vector3f& axis);
  void glRotate(const float angle, const Vector3f& axis);
  void InvertFrom(const Matrix44f& a);
  void InvertInto(Matrix44f *a) const;
  void Print(const char *const name = 0) const;
  static Matrix44f Identity();
  static Matrix44f Zero();
  // x' = M(x)
  Vector4f Transform(const Vector4f& vector) const;
  // x' = M^T(x)
  Vector4f TransformTransposed(const Vector4f& vector) const;
  // x_3' = M_33(x_3)
  Vector3f TransformUpper33(const Vector3f& vector) const;
  // x_3' = M_33^T(x_3)
  Vector3f TransformTransposedUpper33(const Vector3f& vector) const;
  // M = A * B, M(x) = A(B(x))
  void Multiply(const Matrix44f& a, const Matrix44f& b);
  bool EpsilonEqual(const Matrix44f& a, const float epsilon);
  void glOrthof(float l, float r, float b, float t, float n, float f);
};

#endif  // LAMB_MATH_MATRIX44F_H_
