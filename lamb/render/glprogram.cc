/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
    This code is subject to the Google C++ Coding conventions:
        http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
 ******************************************************************************/
#include "lamb/render/glprogram.h"
#include <stdio.h>
#include "lamb/render/glshader.h"
#include "lamb/render/opengl.h"
#include "lamb/log/log.h"

#define DEBUG_PROGRAM
// #define VALIDATE_PROGRAM

GLProgram::GLProgram()
    : program_(0) {
  program_ = glCreateProgram();
}

GLProgram::~GLProgram() {
  glDeleteProgram(program_);
}

GLProgram *GLProgram::FromText(const char *vertex_shader_source,
                               const char *fragment_shader_source) {
  GLShader *vertex_shader = GLShader::LoadAndCompile(GLShader::VERTEX,
                                                     vertex_shader_source);
  GLShader *fragment_shader = GLShader::LoadAndCompile(GLShader::FRAGMENT,
                                                       fragment_shader_source);
  if (!vertex_shader || !fragment_shader) {
    SAFE_DELETE(vertex_shader);
    SAFE_DELETE(fragment_shader);
    return NULL;
  }
  GLProgram *program = new GLProgram();
  program->Attach(vertex_shader);
  program->Attach(fragment_shader);
  SAFE_DELETE(vertex_shader);
  SAFE_DELETE(fragment_shader);
  /*if (program->Link() == false) {
    Log("Failed to link program!\n");
    delete program;
    return NULL;
  }*/
  return program;
}

void GLProgram::Attach(const GLShader *shader) const {
  glAttachShader(program_, shader->shader());
}

bool GLProgram::Link() const {
  glLinkProgram(program_);
#ifdef DEBUG_PROGRAM
  GLint log_length;
  glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &log_length);
  if (log_length > 0) {
    GLchar *log = new GLchar[log_length];
    glGetProgramInfoLog(program_, log_length, &log_length, log);
    Log("*** Program link log:\n%s", log);
    delete[] log;
  }
#endif
  GLint status;
  glGetProgramiv(program_, GL_LINK_STATUS, &status);
  if (status == 0)
    return false;
  return true;
}

void GLProgram::BindAttribLocation(const char *name,
                                   AttributeLocation location) const {
  glBindAttribLocation(program_, location, name);
}

bool GLProgram::Validate() const {
  GLint log_length;
  glValidateProgram(program_);
  glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &log_length);
  if (log_length > 0) {
    GLchar *log = new GLchar[log_length];
    glGetProgramInfoLog(program_, log_length, &log_length, log);
    Log("*** Program validate log:\n%s", log);
    delete []log;
  }
  GLint status;
  glGetProgramiv(program_, GL_VALIDATE_STATUS, &status);
  if (status == 0)
    return false;
  return true;
}

void GLProgram::Use() const {
#ifdef VALIDATE_PROGRAM
  Validate();
#endif
  glUseProgram(program_);
}

void GLProgram::Disable() const {
  glUseProgram(0);
}

UniformLocation GLProgram::GetUniformLocation(const char *name) const {
  return glGetUniformLocation(program_, name);
}

UniformLocation GLProgram::GetAttribLocation(const char *name) const {
  return glGetAttribLocation(program_, name);
}

