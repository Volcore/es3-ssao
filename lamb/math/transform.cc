/*******************************************************************************
    Copyright (c) 2012, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#include "lamb/math/transform.h"

void Transform::LookAtMatrix(const Vector3f &pos, const Vector3f &dir, const Vector3f &up, const Vector3f &scale) {
  Vector3f d(dir);
  Vector3f r;
  r.Cross(d, up);
  r.Normalize();
  Vector3f u;
  u.Cross(r, d);
  const float *p = &pos.x;
  float *m = matrix_.m;
  m[ 0] = r.x*scale.x;
  m[ 4] = u.x*scale.y;
  m[ 8] = -d.x*scale.z;
  m[12] = p[0];

  m[ 1] = r.y*scale.x;
  m[ 5] = u.y*scale.y;
  m[ 9] = -d.y*scale.z;
  m[13] = p[1];

  m[ 2] = r.z*scale.x;
  m[ 6] = u.z*scale.y;
  m[10] = -d.z*scale.z;
  m[14] = p[2];

  m[ 3] = 0.0f;
  m[ 7] = 0.0f;
  m[11] = 0.0f;
  m[15] = 1.0f;
}

