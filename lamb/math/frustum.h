/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_MATH_FRUSTUM_H_
#define LAMB_MATH_FRUSTUM_H_

#include "lamb/math/vector.h"

class Frustum {
 public:
  Frustum();
  void set_plane(int i, const Vector4f &plane);
  const Vector4f& plane(int i) { return planes_[i]; }
  bool IsAABBInside(const Vector3f& min, const Vector3f& max);
  bool IsAABBInsideFast(const float minmax[2][3]);
  bool IsPointInside(const Vector3f& point);
 private:
  Vector4f planes_[6];
  // small optimization. precomputed min/max affinity of plane
  int min_max_[6][3];
};

#endif  // LAMB_MATH_FRUSTUM_H_
