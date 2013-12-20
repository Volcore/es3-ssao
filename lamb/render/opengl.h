/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_RENDER_OPENGL_H_
#define LAMB_RENDER_OPENGL_H_

#ifdef __APPLE__
   #include "TargetConditionals.h"
#endif

#if TARGET_OS_IPHONE
  #include <OpenGLES/ES3/gl.h>
  #include <OpenGLES/ES3/glext.h>
  //  #define USE_VAO
  #define FRAGMENT_SHADER "fragment_shader"
#elif TARGET_OS_MAC
  #include <OpenGL/gl.h>
  #include <OpenGL/glext.h>
  //  #define USE_VAO
  #define FRAGMENT_SHADER "fragment_shader_osx"
#endif

#if TARGET_OS_IPHONE
  #define glGenVertexArrays glGenVertexArrays
  #define glBindVertexArray glBindVertexArray
  #define glDeleteVertexArrays glDeleteVertexArrays
  #define GL_TEXTURE_MAX_LOD_LIMBIC GL_TEXTURE_MAX_LEVEL_APPLE
#elif TARGET_OS_MAC
  #define glGenVertexArrays glGenVertexArraysAPPLE
  #define glBindVertexArray glBindVertexArrayAPPLE
  #define glDeleteVertexArrays glDeleteVertexArraysAPPLE
  #define GL_TEXTURE_MAX_LOD_LIMBIC GL_TEXTURE_MAX_LOD
#endif


#endif  // LAMB_RENDER_OPENGL_H_
