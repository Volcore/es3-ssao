/*******************************************************************************
    Copyright (c) 2009, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#include "lamb/math/camera.h"
#include <stdio.h>
#include <cmath>
#include "lamb/math/math.h"


Camera::Camera()
    : view_dirty_(true),
      projection_dirty_(true),
      view_(Matrix44f::Identity()),
      inv_view_(Matrix44f::Identity()),
      projection_(Matrix44f::Identity()),
      inv_projection_(Matrix44f::Identity()),
      viewprojection_(Matrix44f::Identity()),
      inv_viewprojection_(Matrix44f::Identity()),
      position_(0.0f, 0.0f, 0.0f),
      look_at_(0.0f, 0.0f, 0.0f),
      up_(0.0f, 0.0f, 1.0f),
      window_size_(320.0f, 480.0f),
      fov_(90.0f),
      aspect_(320.0f/480.0f),
      near_(1.0f),
      far_(100.0f),
      roll_(0.0f),
      ortho_(false),
      ortho_window_(-1.0f, 1.0f, 1.0f, -1.0f),
      uses_virtual_viewport_(false),
      reference_width_(480.0f),
      reference_height_(320.0f),
      real_width_(480.0f),
      real_height_(320.0f),
      real_csf_(1.0f),
      virtual_width_(480.0f),
      virtual_height_(320.0f),
      virtual_csf_(1.0f),
      virtual_offset_x_(0.0f),
      virtual_offset_y_(0.0f),
      virtual_to_real_ratio_(1.0f) {
}

Camera::~Camera() {
}

void Camera::set_fov(float fov) {
  fov_ = fov;
  ortho_ = false;
  projection_dirty_ = true;
  view_dirty_ = true;
}

void Camera::set_near_far(float near, float far) {
  near_ = near;
  far_ = far;
  projection_dirty_ = true;
  view_dirty_ = true;
}

void Camera::set_window_size(float x, float y) {
  window_size_ = Vector2f(x, y);
  aspect_ = x/y;
  projection_dirty_ = true;
  view_dirty_ = true;
}

void Camera::set_roll(float roll) {
  roll_ = roll;
  view_dirty_ = true;
}

void Camera::BuildViewMatrix() {
  // Compute direction
  Vector3f dir, d;
  dir.Sub(look_at_, position_);
  d.NormalizeFrom(dir);
  // compute right vector
  Vector3f right, r;
  right.Cross(d, up_);
  r.NormalizeFrom(right);
  // compute up vector
  Vector3f u;
  u.Cross(r, d);
  // Apply roll by rotating up and right around forward vector
  // In ortho mode, this is applied to the projection matrix instead
  if (ortho_ == false) {
    float c_up = cosf(roll_);
    float s_up = sinf(roll_);
    r.x = r.x*c_up + u.x*s_up;
    r.y = r.y*c_up + u.y*s_up;
    r.z = r.z*c_up + u.z*s_up;
    // regenerate the up vector again
    u.Cross(r, d);
  }
  real_right_ = r;
  real_up_ = u;
  // compute matrix
  float *p = &position_.x;
  float tx  = p[0]*r.x+p[1]*r.y+p[2]*r.z;
  float ty  = p[0]*u.x+p[1]*u.y+p[2]*u.z;
  float tz  = -p[0]*d.x-p[1]*d.y-p[2]*d.z;
  float *v = view_.m;
  v[ 0] = r.x;     v[ 4] = r.y;     v[ 8] = r.z;     v[12] = -tx;   // NOLINT
  v[ 1] = u.x;     v[ 5] = u.y;     v[ 9] = u.z;     v[13] = -ty;   // NOLINT
  v[ 2] = -d.x;    v[ 6] = -d.y;    v[10] = -d.z;    v[14] = -tz;   // NOLINT
  v[ 3] = 0.0f;    v[ 7] = 0.0f;    v[11] = 0.0f;    v[15] = 1.0f;  // NOLINT
  // compute inverse
  inv_view_.InvertFrom(view_);
  // reset update flag
  view_dirty_ = false;
  // build the composite matrices and frustum
  if (projection_dirty_)
    BuildProjMatrix();
  // concatenate matrices
  viewprojection_.Multiply(projection_, view_);
  inv_viewprojection_.Multiply(inv_view_, inv_projection_);
  // compute the world space frustum
  for (int i = 0; i < 6; ++i) {
    // transform every plane
    Vector4f p = frustum_.plane(i);
    Vector3f x = Vector3f(p.x, p.y, p.z);
    Vector3f r = view_.TransformTransposedUpper33(x);
    Vector3f m(view_.m[12], view_.m[13], view_.m[14]);
    p.x = r.x;
    p.y = r.y;
    p.z = r.z;
    p.w = p.w - Vector3f::Dot(x, m);
    ws_frustum_.set_plane(i, p);
  }
}

void Camera::set_position(const Vector3f& p) {
  position_ = p;
  view_dirty_ = true;
}

void Camera::set_look_at(const Vector3f& l) {
  look_at_ = l;
  view_dirty_ = true;
}

void Camera::set_up(const Vector3f& u) {
  up_ = u;
  view_dirty_ = true;
}

void Camera::set_ortho(float left, float right, float bottom, float top) {
  ortho_ = true;
  ortho_window_.x = left;
  ortho_window_.y = right;
  ortho_window_.z = bottom;
  ortho_window_.w = top;
}

void Camera::BuildProjMatrix() {
  if (ortho_) {
    float l = ortho_window_.x;
    float r = ortho_window_.y;
    float b = ortho_window_.z;
    float t = ortho_window_.w;
    float n = near_;
    float f = far_;
    float *p = projection_.m;
    float tx  = -(r+l)/(r-l);
    float ty  = -(t+b)/(t-b);
    float tz  = -(f+n)/(f-n);
    float roll = roll_;
    float c_up = cosf(roll);
    float s_up = sinf(roll);
    float x = 2.0f/(r-l);
    float y = 2.0f/(t-b);
    //  c  s 0 0     x 0 0 tx      c*x  s*y 0  c*tx+s*ty
    // -s  c 0 0  \/ 0 y 0 ty  -- -s*x  c*y 0 -s*tx+c*ty
    //  0  0 1 0  /\ 0 0 z tz  --   0    0  z     tz
    //  0  0 0 1     0 0 0 1        0    0  0      1
    // left multiplied we find a rotation around the z axis about roll radians
    p[0] = x*c_up;      p[ 4] = y*s_up;     p[ 8] = 0.0f;                   p[12] = tx*c_up+ty*s_up;   // NOLINT
    p[1] = -x*s_up;     p[ 5] = y*c_up;     p[ 9] = 0.0f;                   p[13] = -tx*s_up+ty*c_up;  // NOLINT
    p[2] = 0.0f;        p[ 6] = 0.0f;       p[10] = -2.0f/(f-n);            p[14] = tz;                // NOLINT
    p[3] = 0.0f;        p[ 7] = 0.0f;       p[11] = 0.0f;                   p[15] = 1.0f;              // NOLINT
  } else {
    float rfov = deg2Rad(fov_/2.0f);
    float t = tanf(rfov);
    float f = 1.0f/t;
    float *p    = projection_.m;
    p[ 0] = f/aspect_;  p[ 4] = 0.0f;       p[ 8] = 0.0f;                   p[12] = 0.0f;                           // NOLINT
    p[ 1] = 0.0f;       p[ 5] = f;          p[ 9] = 0.0f;                   p[13] = 0.0f;                           // NOLINT
    p[ 2] = 0.0f;       p[ 6] = 0.0f;       p[10] = (far_+near_)/(near_-far_);  p[14] = 2*far_*near_/(near_-far_);  // NOLINT
    p[ 3] = 0.0f;       p[ 7] = 0.0f;       p[11] = -1.0f;                  p[15] = 0.0f;                           // NOLINT
  }
  inv_projection_.InvertFrom(projection_);
  this->projection_dirty_ = false;

  // Extract the projetion frustum
  float *p    = projection_.m;
  Vector4f left(-(p[3]+p[0]), -(p[7]+p[4]), -(p[11]+p[8]), (p[15]+p[12]));
  left.NormalizePlane();
  frustum_.set_plane(0, left);

  Vector4f right(-(p[3]-p[0]), -(p[7]-p[4]), -(p[11]-p[8]), (p[15]-p[12]));
  right.NormalizePlane();
  frustum_.set_plane(1, right);

  Vector4f top(-(p[3]-p[1]), -(p[7]-p[5]), -(p[11]-p[9]), (p[15]-p[13]));
  top.NormalizePlane();
  frustum_.set_plane(2, top);

  Vector4f bottom(-(p[3]+p[1]), -(p[7]+p[5]), -(p[11]+p[9]), (p[15]+p[13]));
  bottom.NormalizePlane();
  frustum_.set_plane(3, bottom);

  Vector4f near(-(p[3]+p[2]), -(p[7]+p[6]), -(p[11]+p[10]), (p[15]+p[14]));
  near.NormalizePlane();
  frustum_.set_plane(4, near);

  Vector4f far(-(p[3]-p[2]), -(p[7]-p[6]), -(p[11]-p[10]), (p[15]-p[14]));
  far.NormalizePlane();
  frustum_.set_plane(5, far);
}

float *Camera::view() {
  if (view_dirty_)
    BuildViewMatrix();
  return view_.m;
}

float *Camera::projection() {
  if (projection_dirty_)
    BuildProjMatrix();
  return projection_.m;
}

float *Camera::inv_view() {
  if (view_dirty_)
    BuildViewMatrix();
  return inv_view_.m;
}

float *Camera::inv_projection() {
  if (projection_dirty_)
    BuildProjMatrix();
  return inv_projection_.m;
}

float *Camera::viewprojection() {
  if (view_dirty_)
    BuildViewMatrix();
  return viewprojection_.m;
}

float *Camera::inv_viewprojection() {
  if (view_dirty_)
    BuildViewMatrix();
  return inv_viewprojection_.m;
}

const Vector3f& Camera::position() {
  return position_;
}

const Vector3f& Camera::look_at() {
  return look_at_;
}

const Vector3f& Camera::up() {
  return up_;
}

const Vector2f& Camera::window_size() {
  return window_size_;
}

Vector4f Camera::ScreenToWorld(const Vector4f &screen) {
  float w2 = window_size_.x/2.0f;
  float h2 = window_size_.y/2.0f;
  // screen to NDC
  Vector4f ndc;
  ndc.x = (screen.x-w2)/w2;
  ndc.y = (h2-screen.y)/h2;
  ndc.z = 2.0f*screen.z-1.0f;
  ndc.w = screen.w;
  Vector4f cam = inv_projection_.Transform(ndc);
  Vector4f proj = inv_view_.Transform(cam);
  return Vector4f(proj.x/proj.w, proj.y/proj.w, proj.z/proj.w, 1.0f);
}

Vector4f Camera::WorldToScreen(const Vector4f &world) {
  Vector4f cam  = view_.Transform(world);
  Vector4f clip = projection_.Transform(cam);
  // dehomo
  clip.x /= clip.w;
  clip.y /= clip.w;
  clip.z /= clip.w;
  clip.w = 1.0f;
  float w2 = window_size_.x/2.0f;
  float h2 = window_size_.y/2.0f;
  Vector4f screen;
  screen.x = clip.x*w2+w2;
  screen.y = h2-clip.y*h2;
  screen.z = 0.5f*clip.z+0.5f;
  screen.w = 1.0f;
  return screen;
}

void Camera::Translate(const Vector3f& d) {
  this->position_.Add(this->position_, d);
  this->view_dirty_ = true;
}

void Camera::TranslateLookAt(const Vector3f& d) {
  this->look_at_.Add(this->look_at_, d);
  this->view_dirty_ = true;
}

Frustum* Camera::frustum() {
  if (this->projection_dirty_)
    BuildProjMatrix();
  return &frustum_;
}

Frustum* Camera::world_space_frustum() {
  if (this->projection_dirty_)
    BuildProjMatrix();
  if (this->view_dirty_)
    BuildViewMatrix();
  return &ws_frustum_;
}

void Camera::DisableVirtualViewport() {
  uses_virtual_viewport_ = false;
}

void Camera::SetVirtualViewport(float reference_width, float reference_height, float real_width, float real_height, float real_csf, VirtualViewportMode mode) {
  uses_virtual_viewport_ = true;
  reference_width_ = reference_width;
  reference_height_ = reference_height;
  real_width_ = real_width;
  real_height_ = real_height;
  real_csf_ = real_csf;
  // Compute the ratios wrt reference display size
  float wratio = real_width_/reference_width_;
  float hratio = real_height_/reference_height_;
  switch (mode) {
  case kVirtualViewportMode_CoverAll: {
    if (wratio > hratio) {
      virtual_to_real_ratio_ = wratio;
    } else {
      virtual_to_real_ratio_ = hratio;
    }
    break;
  }
  case kVirtualViewportMode_ShowHeight: {
    virtual_to_real_ratio_ = hratio;
    break;
  }
  case kVirtualViewportMode_ShowWidth: {
    virtual_to_real_ratio_ = wratio;
    break;
  }
  case kVirtualViewportMode_ShowAll: {
    if (wratio < hratio) {
      virtual_to_real_ratio_ = wratio;
    } else {
      virtual_to_real_ratio_ = hratio;
    }
    break;
  }
  }
  virtual_width_ = real_width_/virtual_to_real_ratio_;
  virtual_height_ = real_height_/virtual_to_real_ratio_;
  virtual_offset_x_ = (virtual_width_-reference_width_)/2.0f;
  virtual_offset_y_ = (virtual_height_-reference_height_)/2.0f;
  virtual_csf_ = real_csf_*virtual_to_real_ratio_;
  // Update the camera
  set_window_size(virtual_width_, virtual_height_);
}

Vector2f Camera::RealToVirtual(float x, float y) {
  return Vector2f(x/virtual_to_real_ratio_-virtual_offset_x_, y/virtual_to_real_ratio_-virtual_offset_y_);
}

Vector2f Camera::VirtualToReal(float x, float y) {
  return Vector2f((x+virtual_offset_x_)*virtual_to_real_ratio_, (y+virtual_offset_y_)*virtual_to_real_ratio_);
}

void Camera::GetVirtualScreenRay(float x, float y, Vector3f *origin, Vector3f *direction) {
  Vector4f end(ScreenToWorld(Vector4f(x+virtual_offset_x_, y+virtual_offset_y_, 1, 1)));
  *origin = position_;
  direction->NormalizedDirectionFromTo(position_, Vector3f(end.x, end.y, end.z));
}

void Camera::WorldToVirtualScreen(const Vector3f &world, Vector2f *screen) {
  Vector4f end(WorldToScreen(Vector4f(world.x, world.y, world.z, 1)));
  screen->x = end.x - virtual_offset_x_;
  screen->y = virtual_height_ - (end.y - virtual_offset_y_);
}

void Camera::FromFloats(const float *pm, const float *vm) {
  projection_dirty_ = false;
  view_dirty_ = false;
  projection_ = Matrix44f(pm);
  inv_projection_.InvertFrom(projection_);
  view_ = Matrix44f(vm);
  inv_view_.InvertFrom(view_);
  viewprojection_.Multiply(projection_, view_);
  inv_viewprojection_.Multiply(inv_view_, inv_projection_);
}
