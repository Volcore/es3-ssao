/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#include "lamb/math/vector2f.h"

void Vector2f::Print(const char *const name) const {
  if (name != 0) {
    Log("Vector2f \"%s\"\n", name);
  }
  Log("%f %f\n", x, y);
}
