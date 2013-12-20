/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#include "lamb/math/frustum.h"

Frustum::Frustum() {}

void Frustum::set_plane(int i, const Vector4f &plane) {
  planes_[i] = plane;
  // Precompute plane affinity to avoid if statement in AABBInside function
  min_max_[i][0] = plane.x <= 0.0f;
  min_max_[i][1] = plane.y <= 0.0f;
  min_max_[i][2] = plane.z <= 0.0f;
}

bool Frustum::IsAABBInside(const Vector3f& min, const Vector3f& max) {
  float minmax[2][3] = {{min.x, min.y, min.z}, {max.x, max.y, max.z}};
  for (int i = 0; i < 6; ++i) {
    Vector4f &p = planes_[i];
    float x = minmax[min_max_[i][0]][0];
    float y = minmax[min_max_[i][1]][1];
    float z = minmax[min_max_[i][2]][2];
    float dist = p.x*x+p.y*y+p.z*z-p.w;
    if (dist > 0.0f)
      return false;
  }
  return true;
}

bool Frustum::IsAABBInsideFast(const float minmax[2][3]) {
  for (int i = 0; i < 6; ++i) {
    Vector4f &p = planes_[i];
    float x = minmax[min_max_[i][0]][0];
    float y = minmax[min_max_[i][1]][1];
    float z = minmax[min_max_[i][2]][2];
    float dist = p.x*x+p.y*y+p.z*z-p.w;
    if (dist > 0.0f)
      return false;
  }
  return true;
}


bool Frustum::IsPointInside(const Vector3f& v) {
  for (int i = 0; i < 6; ++i) {
    Vector4f &p = planes_[i];
    float dist = p.x*v.x+p.y*v.y+p.z*v.z-p.w;
    if (dist > 0) return false;
  }
  return true;
}
