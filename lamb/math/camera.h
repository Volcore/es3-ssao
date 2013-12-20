/*******************************************************************************
    Copyright (c) 2009, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_MATH_CAMERA_H_
#define LAMB_MATH_CAMERA_H_

#include "lamb/math/vector.h"
#include "lamb/math/matrix.h"
#include "lamb/math/frustum.h"

enum VirtualViewportMode {
  // Makes sure that the entire display is covered with the virtual screen
  kVirtualViewportMode_CoverAll = 0,
  // Makes sure the entire display width is covered
  kVirtualViewportMode_ShowWidth = 1,
  // Makes sure the entire display height is covered
  kVirtualViewportMode_ShowHeight = 2,
  // Makes sure the entire virtual screen fits into the display
  kVirtualViewportMode_ShowAll = 3,
};

class Camera {
 public:
  Camera();
  ~Camera();
  float *view();
  float *projection();
  float *inv_view();
  float *inv_projection();
  float *viewprojection();
  float *inv_viewprojection();
  const Vector3f& position();
  const Vector3f& look_at();
  const Vector3f& up();
  const Vector2f& window_size();
  float roll() { return roll_; }
  void set_position(const Vector3f& p);
  void set_look_at(const Vector3f& l);
  void set_up(const Vector3f& u);
  void set_fov(float fov);
  void set_near_far(float near, float far);
  void set_roll(float roll);
  void set_ortho(float left, float right, float top, float bottom);
  void FromFloats(const float *pm, const float *vm);
  void SetVirtualViewport(float reference_width, float reference_height, float real_width, float real_height, float real_csf, VirtualViewportMode mode = kVirtualViewportMode_ShowAll);
  void DisableVirtualViewport();
  void Translate(const Vector3f& d);
  void TranslateLookAt(const Vector3f& d);
  Vector4f ScreenToWorld(const Vector4f &screen);
  Vector4f WorldToScreen(const Vector4f &world);
  Vector2f RealToVirtual(float x, float y);
  Vector2f VirtualToReal(float x, float y);
  Frustum* frustum();
  Frustum* world_space_frustum();
  bool uses_virtual_viewport() const { return uses_virtual_viewport_; }
  float virtual_width() const { return virtual_width_; }
  float virtual_height() const { return virtual_height_; }
  float reference_width() const { return reference_width_; }
  float reference_height() const { return reference_height_; }
  float virtual_offset_x() const { return virtual_offset_x_; }
  float virtual_offset_y() const { return virtual_offset_y_; }
  float virtual_csf() const { return virtual_csf_; }
  float virtual_to_real_ratio() const { return virtual_to_real_ratio_; }
  float real_width() const { return real_width_; }
  float real_height() const { return real_height_; }
  float real_csf() const { return real_csf_; }
  float near() const { return near_; }
  float far() const { return far_; }
  const Vector3f &real_right() const { return real_right_; }
  const Vector3f &real_up() const { return real_up_; }
  void GetVirtualScreenRay(float x, float y, Vector3f *origin, Vector3f *direction);
  void WorldToVirtualScreen(const Vector3f &world, Vector2f *screen);

 private:
  void set_window_size(float x, float y);
  void BuildViewMatrix();
  void BuildProjMatrix();
  void BuildMatrices();
  bool view_dirty_;
  bool projection_dirty_;
  Matrix44f view_;
  Matrix44f inv_view_;
  Matrix44f projection_;
  Matrix44f inv_projection_;
  Matrix44f viewprojection_;
  Matrix44f inv_viewprojection_;
  Vector3f position_;
  Vector3f look_at_;
  Vector3f up_;
  Vector2f window_size_;
  float fov_;
  float aspect_;
  float near_;
  float far_;
  Frustum frustum_;
  Frustum ws_frustum_;
  float roll_;
  bool ortho_;
  Vector4f ortho_window_;
  float content_scale_factor_;
  ////////////////////////////////
  // Real vectors are the normalized
  // vectors of the image plane
  Vector3f real_right_ = Vector3f(1.0f, 0.0f, 0.0f);
  Vector3f real_up_ = Vector3f(0.0f, 0.0f, 1.0f);
  ////////////////////////////////
  // Virtual viewport data
  bool uses_virtual_viewport_;
  // the width & height of the reference viewport, the viewport without proper pixel scaling and aspect ratio
  float reference_width_;
  float reference_height_;
  // the actual width & height & csf of the display
  float real_width_;
  float real_height_;
  float real_csf_;
  // the virtual width & height & csf of the display. this scales up pixels to match reference_height_ and real_height_, and scales width accordingly to maintain the aspect ratio of the real display.
  float virtual_width_;
  float virtual_height_;
  float virtual_csf_;
  // offsets ot keep the virtual scaled up reference view centered in the real display
  float virtual_offset_x_;
  float virtual_offset_y_;
  // this is the ratio between reference_height_ and real_height_
  float virtual_to_real_ratio_;
};

#endif  // LAMB_MATH_CAMERA_H_
