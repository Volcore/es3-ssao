/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
    This code is subject to the Google C++ Coding conventions:
        http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
 ******************************************************************************/
#include "lamb/render/vbomodel.h"
#include <vector>
#include <stdio.h>
#include "lamb/render/pvrfile.h"

#pragma pack(push, 1)
struct VBOHeader {
  int32_t version;
  struct {
    uint8_t num;
    uint32_t type;
    uint32_t offset;
    uint32_t stride;
  } attribs[3];
  uint32_t vbo_size;
  uint32_t ibo_type;
  uint32_t ibo_data_type;
  uint32_t ibo_size;
  uint32_t ibo_offset;
};
#pragma pack(pop)

VBOModel::VBOModel()
    : vbo_(0),
      idx_count_(0) {
}

VBOModel::~VBOModel() {
  SAFE_DELETE(vbo_);
}

void VBOModel::Draw() {
  vbo_->Draw(ibo_type_, idx_count_, ibo_data_type_, 0);
}

VBOModel *VBOModel::Load(const char *const vbo_name) {
  NSString *file = [[NSBundle mainBundle] pathForResource:@(vbo_name) ofType:@"vbo"];
  FILE *fp = fopen(file.UTF8String, "rb");
  if (fp == 0) {
    return 0;
  }
  VBOHeader header;
  fread(&header, 1, sizeof(VBOHeader), fp);
  if (header.version != 4) {
    return 0;
  }
  std::vector<uint8_t> vbo_data(header.vbo_size);
  fread(vbo_data.data(), 1, header.vbo_size, fp);
  std::vector<uint8_t> ibo_data(header.ibo_size);
  fread(ibo_data.data(), 1, header.ibo_size, fp);
  fclose(fp);
  VBOModel *model = new VBOModel();
  // Load the VBO
  // Shader attributes have to be in the order
  //    position
  //    texcoords
  model->vbo_ = new VertexBufferObject(true);
  model->vbo_->SetVertexData(vbo_data.data(), vbo_data.size());
  for (int i = 0; i < 1; ++i) {
    // For this demo, skip the second component (texcoords)
    model->vbo_->AddAttribute(i, header.attribs[i].num, header.attribs[i].type, false, header.attribs[i].stride, header.attribs[i].offset);
  }
  model->vbo_->SetIndexData(ibo_data.data(), ibo_data.size());
  switch (header.ibo_data_type) {
  default:
  case GL_UNSIGNED_BYTE: model->idx_count_ = header.ibo_size; break;
  case GL_UNSIGNED_SHORT: model->idx_count_ = header.ibo_size / 2; break;
  case GL_UNSIGNED_INT: model->idx_count_ = header.ibo_size / 4; break;
  }
  model->ibo_data_type_ = header.ibo_data_type;
  model->ibo_type_ = header.ibo_type;
  return model;
}

