/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
    This code is subject to the Google C++ Coding conventions:
        http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
 ******************************************************************************/
#include "lamb/render/vertexbufferobject.h"
#include <stdio.h>

VertexBufferObject::VertexBufferObject(bool use_vao)
    : use_vao_(use_vao) {
  if (use_vao_) {
    glGenVertexArrays(1, &vao_id_);
    glBindVertexArray(vao_id_);
  }
}

VertexBufferObject::~VertexBufferObject() {
  if (use_vao_) {
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao_id_);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &vbo_id_);
  glDeleteBuffers(1, &ibo_id_);
}

void VertexBufferObject::SetVertexData(uint8_t *data, size_t length, bool stream) {
  glGenBuffers(1, &vbo_id_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
  glBufferData(GL_ARRAY_BUFFER, length, data, stream?GL_STREAM_DRAW:GL_STATIC_DRAW);
}

void VertexBufferObject::SetIndexData(uint8_t *data, size_t length, bool stream) {
  glGenBuffers(1, &ibo_id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, length, data, stream?GL_STREAM_DRAW:GL_STATIC_DRAW);
}

void VertexBufferObject::AddAttribute(int attribute, int count, int type, bool normalize, int stride, int offset) {
  if (use_vao_) {
    glEnableVertexAttribArray(attribute);
    glVertexAttribPointer(attribute, count, type, normalize, stride, reinterpret_cast<GLvoid*>(offset));
  } else {
    attributes_.push_back(Attribute());
    attributes_.back().attribute = attribute;
    attributes_.back().count = count;
    attributes_.back().type = type;
    attributes_.back().normalize = normalize;
    attributes_.back().stride = stride;
    attributes_.back().offset = offset;
  }
}

void VertexBufferObject::Bind() {
  if (vao_id_) {
    glBindVertexArray(vao_id_);
  } else {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id_);
    auto ait = attributes_.begin();
    while (ait != attributes_.end()) {
      glEnableVertexAttribArray(ait->attribute);
      glVertexAttribPointer(ait->attribute, ait->count, ait->type, ait->normalize, ait->stride, reinterpret_cast<GLvoid*>(ait->offset));
      ++ait;
    }
  }
}

void VertexBufferObject::Unbind() {
  if (vao_id_) {
    glBindVertexArray(0);
  } else {
    auto ait = attributes_.begin();
    while (ait != attributes_.end()) {
      glDisableVertexAttribArray(ait->attribute);
      ++ait;
    }
  }
}


void VertexBufferObject::Draw(unsigned int type, unsigned int count, unsigned int data_type, int offset) {
  Bind();
  glDrawElements(type, count, data_type, reinterpret_cast<GLvoid*>(offset));
  Unbind();
}
