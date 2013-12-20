/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_RENDER_FRAMEBUFFEROBJECT_H_
#define LAMB_RENDER_FRAMEBUFFEROBJECT_H_

#include "lamb/codingguides.h"

enum FramebufferFormat {
  kFBO_None,
  kFBO_R8,
  kFBO_RGB8,
  kFBO_RGB16,
  kFBO_RGBA8,
};

enum DepthFormat {
  kFBODepth_None,
  kFBODepth_Texture16,
  kFBODepth_Texture24,
};

class FramebufferObject {
 public:
  ~FramebufferObject();
  static FramebufferObject *Create(int width, int height, FramebufferFormat format, FramebufferFormat format2, FramebufferFormat format3, DepthFormat depth);
  void Activate();
  void Deactivate();
  unsigned int fbo_id() const { return fbo_id_; }
  unsigned int tex_id(int i) const { return tex_id_[i]; }
  unsigned int depth_id() const { return depth_id_; }

 private:
  FramebufferObject();
  int width_ = 1;
  int height_ = 1;
  unsigned int fbo_id_ = 0;
  unsigned int tex_id_[4] = { 0 };
  unsigned int depth_id_ = 0;
  unsigned int num_drawbuffs_ = 0;
  unsigned int drawbuffs_[4] = { 0 };
  // Temporary variables used for rendering.
  int old_fbo_ = 0;
  int old_viewport_[4] = { 0 };
  DISALLOW_COPY_AND_ASSIGN(FramebufferObject);
};

#endif  // LAMB_RENDER_FRAMEBUFFEROBJECT_H_
