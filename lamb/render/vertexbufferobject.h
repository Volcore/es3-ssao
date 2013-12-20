/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
    This code is subject to the Google C++ Coding conventions:
        http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
 ******************************************************************************/
#ifndef LAMB_RENDER_VERTEXBUFFEROBJECT_H_
#define LAMB_RENDER_VERTEXBUFFEROBJECT_H_

#include <stdint.h>
#include <list>
#include "lamb/codingguides.h"
#include "lamb/render/opengl.h"

class VertexBufferObject {
 public:
  VertexBufferObject(bool use_vao);
  ~VertexBufferObject();
  void SetVertexData(uint8_t *data, size_t length, bool stream = false);
  void SetIndexData(uint8_t *data, size_t length, bool stream = false);
  void AddAttribute(int attribute, int count, int type, bool noramlize, int stride, int offset);
  void Draw(unsigned int type, unsigned int count, unsigned int data_type, int offset);
  void Bind();
  void Unbind();

 private:
  unsigned int vbo_id_ = 0;
  unsigned int ibo_id_ = 0;
  unsigned int vao_id_ = 0;
  struct Attribute {
    int attribute;
    int count;
    int type;
    bool normalize;
    int stride;
    int offset;
  };
  std::list<Attribute> attributes_;
  bool use_vao_ = false;
  DISALLOW_COPY_AND_ASSIGN(VertexBufferObject);
};

#endif  // LAMB_RENDER_VERTEXBUFFEROBJECT_H_
