/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_MATH_MATRIX33F_H_
#define LAMB_MATH_MATRIX33F_H_

#include "lamb/codingguides.h"

struct Vector2f;
struct Vector3f;
struct Matrix44f;

struct Matrix33f {
  // Human Readable Matrix
  //  m00 m01 m02
  //  m10 m11 m12
  //  m20 m21 m22
  // is stored in memory as
  //  m00 m10 m20 m01 m11 m21 ...
  // so in the column-major order
  //   0   3   6
  //   1   4   7
  //   2   5   8
  union {
    struct {
      float m00, m10, m20;
      float m01, m11, m21;
      float m02, m12, m22;
    };
    float m[9];
  };
  explicit Matrix33f();
  explicit Matrix33f(float m00, float m01, float m02,
                     float m10, float m11, float m12,
                     float m20, float m21, float m22);
  explicit Matrix33f(const float a[9]);
  // The only non-explicit constructor is the copy constructor
  Matrix33f(const Matrix33f &a);
  ~Matrix33f();
  // Computes M = a b^T; m_ij = a_j * b_i
  void OuterProduct(const Vector3f &a, const Vector3f &bt);
  void CrossProduct(const Vector3f &a);
  static Matrix33f Rotation(const float angle, const Vector3f& axis);
  void Transpose();
  void TransposeFrom(const Matrix33f &a);
  void TransposeInto(Matrix33f *a) const;
  void InvertFrom(const Matrix33f& a);
  void InvertInto(Matrix33f *a) const;
  void Add(const Matrix33f &a, const Matrix33f &b);
  void Scale(float s);
  float RMSE(const Matrix33f &m) const;
  void Print(const char *const name = 0) const;
  float Determinant() const;
  static Matrix33f Identity();
  // x' = M(x)
  Vector3f Transform(const Vector3f& vector) const;
  // x' = M^T(x)
  Vector3f TransformTransposed(const Vector3f& vector) const;
  // x2' = dehomogenize(M([x2, 1]))
  Vector2f Project(const Vector2f& p) const;
  // M = A * B, M(x) = A(B(x))
  void Multiply(const Matrix33f& a, const Matrix33f& b);
  bool EpsilonEqual(const Matrix33f& a, const float epsilon);
};

void MakeNormalMatrix(Matrix33f *dst, const Matrix44f & src);

#endif  // LAMB_MATH_MATRIX33F_H_
