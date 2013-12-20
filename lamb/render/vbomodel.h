/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
    This code is subject to the Google C++ Coding conventions:
        http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
 ******************************************************************************/
#ifndef LAMB_RENDER_VBOMODEL_H_
#define LAMB_RENDER_VBOMODEL_H_

#include "lamb/codingguides.h"
#include "lamb/math/transform.h"
#include "lamb/render/vertexbufferobject.h"
#include "lamb/render/glprogram.h"

class VBOModel {
 public:
  VBOModel();
  ~VBOModel();
  static VBOModel *Load(const char *const vbo_name, bool texture);
  static VBOModel *Cube();
  void Draw();
  unsigned int tri_count() const { return idx_count_/3; }
  VertexBufferObject *vbo() { return vbo_; }
  int idx_count() const { return idx_count_; }
  uint32_t ibo_data_type() const { return ibo_data_type_; }
  uint32_t ibo_type() const { return ibo_type_; }

 private:
  VertexBufferObject *vbo_;
  int idx_count_;
  uint32_t ibo_data_type_;
  uint32_t ibo_type_;
  DISALLOW_COPY_AND_ASSIGN(VBOModel);
};

#endif  // LAMB_RENDER_VBOMODEL_H_
