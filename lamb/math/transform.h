/*******************************************************************************
    Copyright (c) 2009, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_MATH_TRANSFORM_H_
#define LAMB_MATH_TRANSFORM_H_

#include "lamb/math/matrix44f.h"
#include "lamb/math/vector3f.h"

/* How to use:
 
      x' = S R T x
 
      Human way:
        Transform transform;
        transform.ThenTranslate();
        transform.ThenRotate();
        transform.ThenScale();
 
      OpenGL way:
        Transform transform;
        transform.glScale();
        transform.glRotate();
        transform.glTranslate();
 
 */

class Transform {
 public:
  inline Transform()
      : matrix_(Matrix44f::Identity()) {
  }
  explicit inline Transform(const Matrix44f& matrix)
      : matrix_(matrix) {
  }
  // Static constructors
  static inline Transform Translate(const Vector3f& x) {
    return Transform(Matrix44f::Translation(x));
  }
  static inline Transform Translate(float x, float y, float z) {
    return Transform(Matrix44f::Translation(x, y, z));
  }
  static inline Transform Scale(const Vector3f& s) {
    return Transform(Matrix44f::Scaling(s));
  }
  static inline Transform Scale(float sx, float sy, float sz) {
    return Transform(Matrix44f::Scaling(sx, sy, sz));
  }
  static inline Transform Rotate(float angle, const Vector3f& axis) {
    return Transform(Matrix44f::Rotation(angle, axis));
  }
  // Human way
  inline Transform& ThenTranslate(float x, float y, float z) {
    matrix_.Translate(x, y, z);
    return *this;
  }
  inline Transform& ThenTranslate(const Vector3f& x) {
    return ThenTranslate(x.x, x.y, x.z);
  }
  inline Transform& ThenScale(float x, float y, float z) {
    matrix_.Scale(x, y, z);
    return *this;
  }
  inline Transform& ThenScale(const Vector3f& s) {
    return ThenScale(s.x, s.y, s.z);
  }
  inline Transform& ThenRotate(float angle, const Vector3f& axis) {
    matrix_.Rotate(angle, axis);
    return *this;
  }
  inline Transform& ThenTransform(const Transform& transform) {
    Matrix44f r;
    r.Multiply(transform.matrix_, this->matrix_);
    this->matrix_ = r;
    return *this;
  }
  inline Transform& ThenTransform(const Matrix44f& mat) {
    Matrix44f r;
    r.Multiply(mat, this->matrix_);
    this->matrix_ = r;
    return *this;
  }

  // OpenGL way
  inline Transform& glTranslate(float x, float y, float z) {
    matrix_.glTranslate(x, y, z);
    return *this;
  }
  inline Transform& glTranslate(const Vector3f& x) {
    return glTranslate(x.x, x.y, x.z);
  }
  inline Transform& glRotate(float angle, const Vector3f& axis) {
    matrix_.glRotate(angle, axis);
    return *this;
  }
  inline Transform& glScale(float x, float y, float z) {
    matrix_.glScale(x, y, z);
    return *this;
  }
  inline Transform& glScale(const Vector3f& s) {
    return glScale(s.x, s.y, s.z);
  }
  inline Transform& glScale(float s) {
    return glScale(s, s, s);
  }
  inline Transform& glTransform(const Transform& transform) {
    Matrix44f r;
    r.Multiply(this->matrix_, transform.matrix_);
    this->matrix_ = r;
    return *this;
  }
  inline Transform& glTransform(const Matrix44f& mat) {
    Matrix44f r;
    r.Multiply(this->matrix_, mat);
    this->matrix_ = r;
    return *this;
  }
  inline Transform& glOrtho(float l, float r, float b, float t, float n, float f) {
    this->matrix_.glOrthof(l, r, b, t, n, f);
    return *this;
  }
  void LookAtMatrix(const Vector3f &pos, const Vector3f &dir, const Vector3f &up, const Vector3f &scale);
  inline void Print() { matrix_.Print(); }
  inline const float *raw() const { return matrix_.m; }
  const Matrix44f& matrix() const { return matrix_; }

 private:
  Matrix44f matrix_;
};

#endif  // LAMB_MATH_TRANSFORM_H_
