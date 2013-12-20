/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#include "lamb/render/framebufferobject.h"
#include <stdio.h>
#include <stdlib.h>
#include "lamb/render/opengl.h"

#ifndef GL_RED
#ifdef GL_RED_EXT
#define GL_RED GL_RED_EXT
#endif
#endif

FramebufferObject::FramebufferObject() {
}

FramebufferObject::~FramebufferObject() {
  for (int i = 0; i < 2; ++i) {
    if (tex_id_[i]) {
      glDeleteTextures(1, &tex_id_[i]);
      tex_id_[i] = 0;
    }
  }
  if (depth_id_) {
    glDeleteTextures(1, &depth_id_);
    depth_id_ = 0;
  }
  if (fbo_id_) {
    glDeleteFramebuffers(1, &fbo_id_);
    fbo_id_ = 0;
  }
}

FramebufferObject *FramebufferObject::Create(int width, int height, FramebufferFormat format, FramebufferFormat format2, FramebufferFormat format3, DepthFormat depth) {
  FramebufferObject *fbo = new FramebufferObject();
  fbo->width_ = width;
  fbo->height_ = height;
  // Generate the framebuffer
  glGenFramebuffers(1, &fbo->fbo_id_);
  fbo->Activate();
  // Attach the color
  FramebufferFormat formats[] = {format, format2, format3};
  for (int i = 0; i < 3; ++i) {
    if (formats[i] == kFBO_None) {
      continue;
    }
    glGetError();
    glGenTextures(1, &fbo->tex_id_[i]);
    glBindTexture(GL_TEXTURE_2D, fbo->tex_id_[i]);
    unsigned int f1 = GL_RGB;
    unsigned int f2 = GL_RGB;
    unsigned int type = GL_UNSIGNED_BYTE;
    switch (formats[i]) {
    default:
    case kFBO_R8: f1 = GL_RED; f2 = GL_RED; break;
    case kFBO_RGB8: f1 = GL_RGB; f2 = GL_RGB; break;
    case kFBO_RGBA8: f1 = GL_RGBA; f2 = GL_RGBA; break;
    case kFBO_RGB16: f1 = GL_RGB16F; f2 = GL_RGB; type = GL_HALF_FLOAT; break;
    }
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, f1, width, height, 0, f2, type, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, fbo->tex_id_[i], 0);
    fbo->drawbuffs_[i] = GL_COLOR_ATTACHMENT0+i;
    fbo->num_drawbuffs_++;
    int error = glGetError();
    if (error != GL_NO_ERROR) {
      Log("Error\n");
    }
  }
  // Generate depth
  if (depth != kFBODepth_None) {
    unsigned int dformat = GL_DEPTH_COMPONENT24;
    unsigned int type = GL_UNSIGNED_INT;
    switch (depth) {
    default:
    case kFBODepth_Texture24: dformat = GL_DEPTH_COMPONENT24; type = GL_UNSIGNED_INT; break;
    case kFBODepth_Texture16: dformat = GL_DEPTH_COMPONENT16; type = GL_UNSIGNED_SHORT; break;
    }
    glGenTextures(1, &fbo->depth_id_);
    glBindTexture(GL_TEXTURE_2D, fbo->depth_id_);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, dformat, width, height, 0, GL_DEPTH_COMPONENT, type, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo->depth_id_, 0);
    // Renderbuffer code
//    glGenRenderbuffers(1, &fbo->depthrb_id_);
//    glBindRenderbuffer(GL_RENDERBUFFER, fbo->depthrb_id_);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->depthrb_id_);
//    glBindRenderbuffer(GL_RENDERBUFFER, 0);
  }
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    Log("failed to make complete rtt framebuffer object %x\n", glCheckFramebufferStatus(GL_FRAMEBUFFER));
  }
  fbo->Deactivate();
  return fbo;
}

void FramebufferObject::Activate() {
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &old_fbo_);
  glGetIntegerv(GL_VIEWPORT, old_viewport_);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
  glViewport(0, 0, width_, height_);
  glDrawBuffers(num_drawbuffs_, drawbuffs_);
}

void FramebufferObject::Deactivate() {
  glBindFramebuffer(GL_FRAMEBUFFER, old_fbo_);
  glViewport(old_viewport_[0], old_viewport_[1], old_viewport_[2], old_viewport_[3]);
}
