//
//  ViewController.m
//  ES3 SSAO
//
//  Created by Volker Schönefeld on 18/12/13.
//  Copyright (c) 2013 Limbic. All rights reserved.
//
#import "ViewController.h"
#include <numeric>
#include <list>
#include <vector>
#include <mach/mach_time.h>
#include "lamb/render/glprogram.h"
#include "lamb/math/camera.h"
#include "lamb/render/vbomodel.h"
#include "lamb/render/framebufferobject.h"

static mach_timebase_info_data_t timebase;
double Time() {
  uint64_t t = mach_absolute_time();
  if ( timebase.denom == 0 ) {
    mach_timebase_info(&timebase);
  }
  return ((double)t * 1e-9 * (double)timebase.numer / (double)timebase.denom );
}

NSString *StringFromFile(const char *const name, const char *const ext) {
  NSString *file = [[NSBundle mainBundle] pathForResource:@(name)
                                                   ofType:@(ext)];
  return [NSString stringWithContentsOfFile:file
                                   encoding:NSUTF8StringEncoding
                                      error:nil];
}

float frand() {
  return float(random()) / float(RAND_MAX);
}

float kSSAOSamples[] = {
  -0.94201624f, -0.39906216f,
   0.94558609f, -0.76890725f,
  -0.09418410f, -0.92938870f,
   0.34495938f,  0.29387760f,
  -0.91588581f,  0.45771432f,
  -0.81544232f, -0.87912464f,
  -0.38277543f,  0.27676845f,
   0.97484398f,  0.75648379f,
   0.44323325f, -0.97511554f,
   0.53742981f, -0.47373420f,
  -0.26496911f, -0.41893023f,
   0.79197514f,  0.19090188f,
  -0.24188840f,  0.99706507f,
  -0.81409955f,  0.91437590f,
   0.19984126f,  0.78641367f,
   0.14383161f, -0.14100790f
};

@interface ViewController () {
  IBOutlet UILabel *timer_;
  IBOutlet UISlider *complexity_;
  VBOModel *cube_;
  VBOModel *rect_;
  GLProgram *cube_program_;
  int cube_uni_mvp_;
  int cube_uni_mv_normal_;
  int cube_uni_mv_;
  int cube_uni_color_;
  GLProgram *ssao_program_;
  int ssao_uni_mvp_;
  int ssao_uni_color_tex_;
  int ssao_uni_normal_tex_;
  int ssao_uni_depth_tex_;
  int ssao_uni_samples_;
  int ssao_uni_inv_projection_;
  int ssao_uni_num_samples_;
  std::list<double> frame_times_;
  double last_time_;
  double last_update_time_;
  Camera camera_;
  FramebufferObject *fbo_;
  Vector3f samples_[16];
}
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormatNone;
    self.preferredFramesPerSecond = 60;
    
    [self setupGL];
}

- (void)dealloc
{    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
  if ([self isViewLoaded] && ([[self view] window] == nil)) {
    self.view = nil;
    [self tearDownGL];
    if ([EAGLContext currentContext] == self.context) {
      [EAGLContext setCurrentContext:nil];
    }
    self.context = nil;
  }
  // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
  return YES;
}

- (void)setupGL {
  [EAGLContext setCurrentContext:self.context];
  // Load shaders
  cube_program_ = GLProgram::FromText(StringFromFile("cube", "vsh").UTF8String, StringFromFile("cube", "fsh").UTF8String);
  cube_program_->Link();
  cube_uni_mvp_ = cube_program_->GetUniformLocation("uni_mvp");
  cube_uni_mv_normal_ = cube_program_->GetUniformLocation("uni_mv_normal");
  cube_uni_mv_ = cube_program_->GetUniformLocation("uni_mv");
  cube_uni_color_ = cube_program_->GetUniformLocation("uni_color");
  ssao_program_ = GLProgram::FromText(StringFromFile("ssao", "vsh").UTF8String, StringFromFile("ssao", "fsh").UTF8String);
  ssao_program_->Link();
  ssao_uni_mvp_ = ssao_program_->GetUniformLocation("uni_mvp");
  ssao_uni_color_tex_ = ssao_program_->GetUniformLocation("uni_color_tex");
  ssao_uni_normal_tex_ = ssao_program_->GetUniformLocation("uni_normal_tex");
  ssao_uni_depth_tex_ = ssao_program_->GetUniformLocation("uni_depth_tex");
  ssao_uni_samples_ = ssao_program_->GetUniformLocation("uni_samples");
  ssao_uni_inv_projection_ = ssao_program_->GetUniformLocation("uni_inv_projection");
  ssao_uni_num_samples_ = ssao_program_->GetUniformLocation("uni_num_samples");
  cube_ = VBOModel::Cube();
  rect_ = VBOModel::Load("rect", true);
  // Generate samples
  for (int i = 0; i < 16; ++i) {
    samples_[i].x = frand() * 2 - 1;
    samples_[i].y = frand() * 2 - 1;
    samples_[i].z = frand() * 2 - 1;
    samples_[i].Normalize();
  }
}

- (void)tearDownGL {
  [EAGLContext setCurrentContext:self.context];
  SAFE_DELETE(ssao_program_);
  SAFE_DELETE(cube_program_);
  SAFE_DELETE(cube_);
  SAFE_DELETE(rect_);
  SAFE_DELETE(fbo_);
}

#pragma mark - GLKView and GLKViewController delegate methods

const int kMinComplexity = 1;
const int kMaxComplexity = 16;

- (void)update {
  // Compute animation
  double now = Time();
  // Update camera position
  double angle = now;
  float r = 2.0f;
  camera_.set_up(Vector3f(0.0f, 1.0f, 0.0f));
  camera_.set_position(Vector3f(sin(angle)*r, r*0.75f, cos(angle)*r));
  camera_.set_look_at(Vector3f(0.0f, 0.0f, 0.0f));
  camera_.set_near_far(r * 0.1f, r * 4.0f);
}

- (void)drawScene {
  double now = Time();
  // Draw into the FBO
  if (fbo_ == 0) {
    GLKView *view = (GLKView*)self.view;
    fbo_ = FramebufferObject::Create(int(view.drawableWidth), int(view.drawableHeight), kFBO_RGB8, kFBO_RGB8, kFBO_None, kFBODepth_Texture16);
  }
  fbo_->Activate();
  glClearColor(0.65f, 0.65f, 0.65f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  // Draw the cubes
  cube_program_->Use();
  Vector3f offset[] = {
    Vector3f( 0.0f,  0.75f,  0.0f),
    Vector3f( 0.0f, -0.25f, -1.0f),
    Vector3f( 1.0f, -0.25f,  0.0f),
    Vector3f( 1.0f,  0.75f,  1.0f),
    Vector3f( 0.0f,  -0.25f,  0.0f),
    Vector3f( 0.0f,  -1.25f,  0.0f),
    Vector3f( 0.0f,  -2.25f,  0.0f),
    Vector3f( 1.5f*sin(now*1.234f),  0.25f,  1.5f*cos(now*0.643f)),
  };
  float size[] = {
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.1f,
    1.3f,
    1.5f,
    0.3f,
  };
  Vector3f color[] = {
    COLOR_GMAIL_BLUE.xyz(),
    COLOR_ACTION_YELLOW.xyz(),
    COLOR_QOOP_MINT.xyz(),
    COLOR_RSS_ORANGE.xyz(),
    COLOR_WHITE.xyz(),
    COLOR_WHITE.xyz(),
    COLOR_WHITE.xyz(),
    COLOR_TECHCRUNCH_GREEN.xyz()
  };
  int num = sizeof(offset) / sizeof(offset[0]);
  for (int i = 0; i < num; ++i) {
    glUniform3f(cube_uni_color_, color[i].x, color[i].y, color[i].z);
    Transform t(Matrix44f(camera_.viewprojection()));
    t.glTranslate(offset[i]);
    t.glScale(size[i]);
    glUniformMatrix4fv(cube_uni_mvp_, 1, GL_FALSE, t.raw());
    // Compute the normal matrix
    Matrix33f nm;
    Matrix44f(camera_.inv_view()).Upper3x3(&nm);
    glUniformMatrix3fv(cube_uni_mv_normal_, 1, GL_TRUE, nm.m);
    Transform v(Matrix44f(camera_.view()));
    v.glTranslate(offset[i]);
    v.glScale(size[i]);
    glUniformMatrix4fv(cube_uni_mv_, 1, GL_FALSE, v.raw());
    cube_->Draw();
  }
  {
    // Draw an all-encapsulating cube
    // Use negative scaling to make the normals point inwards
    Transform t(Matrix44f(camera_.viewprojection()));
    t.glScale(-5.0f);
    glUniformMatrix4fv(cube_uni_mvp_, 1, GL_FALSE, t.raw());
    // Compute the normal matrix
    Matrix33f nm;
    Matrix44f(camera_.inv_view()).Upper3x3(&nm);
    glUniformMatrix3fv(cube_uni_mv_normal_, 1, GL_TRUE, nm.m);
    Transform v(Matrix44f(camera_.view()));
    v.glScale(-5.0f);
    glUniformMatrix4fv(cube_uni_mv_, 1, GL_FALSE, v.raw());
    glUniform3f(cube_uni_color_, 1.0f, 1.0f, 1.0f);
    cube_->Draw();
  }
  // TODO(VS): invalidate framebuffer
  fbo_->Deactivate();
  glDisable(GL_DEPTH_TEST);
  // Now perform the SSAO pass
  int complexity = int(lerp(kMinComplexity, complexity_.value, kMaxComplexity));
  // Clear the target buffer to prevent a logical buffer load
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  ssao_program_->Use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_->tex_id(0));
  glUniform1i(ssao_uni_color_tex_, 0);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo_->tex_id(1));
  glUniform1i(ssao_uni_normal_tex_, 1);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, fbo_->depth_id());
  glUniform1i(ssao_uni_depth_tex_, 2);
  for (int i = 0; i < 16; ++i) {
    glUniform2fv(ssao_uni_samples_+i, 1, &kSSAOSamples[i*2]);
  }
  glUniformMatrix4fv(ssao_uni_inv_projection_, 1, GL_FALSE, camera_.inv_projection());
  glUniform1i(ssao_uni_num_samples_, complexity);
  Transform t;
  t.glScale(2.0f);
  glUniformMatrix4fv(ssao_uni_mvp_, 1, 0, t.raw());
  rect_->Draw();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
  float w = view.bounds.size.width;
  float h = view.bounds.size.height;
  camera_.SetVirtualViewport(w, h, w, h, 1);
  [self drawScene];
  // Update frame timer
  double now = Time();
  double diff = now - last_time_;
  last_time_ = now;
  frame_times_.push_back(diff);
  if (frame_times_.size() >= 10) {
    double avg_diff = std::accumulate(frame_times_.begin(), frame_times_.end(), 0.0) / double(frame_times_.size());
    double ms = avg_diff * 1000.0;
    frame_times_.clear();
    timer_.text = [NSString stringWithFormat:@"%.1f ms/frame", ms];
  }
}

- (IBAction)change:(id)sender {
  frame_times_.clear();
}

@end
