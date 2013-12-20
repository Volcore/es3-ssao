/*******************************************************************************
    Copyright (c) 2011, Limbic Software, Inc.
    All rights reserved.
    This code is subject to the Google C++ Coding conventions:
        http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
 ******************************************************************************/
#ifndef LAMB_RENDER_GLPROGRAM_H_
#define LAMB_RENDER_GLPROGRAM_H_

#include "lamb/codingguides.h"

class GLShader;

typedef int UniformLocation;
typedef int AttributeLocation;

class GLProgram {
 public:
  GLProgram();
  ~GLProgram();
  static GLProgram *FromText(const char *vshader, const char *fshader);
  void Attach(const GLShader *shader) const;
  void BindAttribLocation(const char *name, AttributeLocation location) const;
  bool Link() const;
  bool Validate() const;
  void Use() const;
  void Disable() const;
  UniformLocation GetUniformLocation(const char *name) const;
  AttributeLocation GetAttribLocation(const char *name) const;
  unsigned int program() const { return program_; }

 private:
  unsigned int program_;
  DISALLOW_COPY_AND_ASSIGN(GLProgram);
};

#endif  // LAMB_RENDER_GLPROGRAM_H_
