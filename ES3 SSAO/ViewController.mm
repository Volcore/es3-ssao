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

@interface ViewController () {
  IBOutlet UILabel *timer_;
  IBOutlet UISlider *complexity_;
  VBOModel *cube_;
  GLProgram *cube_program_;
  int cube_uni_mvp_;
  int cube_uni_color_;
  std::list<double> frame_times_;
  double last_time_;
  double last_update_time_;
  Camera camera_;
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
  cube_uni_color_ = cube_program_->GetUniformLocation("uni_color");
  cube_ = VBOModel::Load("cube");
}

- (void)tearDownGL {
  [EAGLContext setCurrentContext:self.context];
  SAFE_DELETE(cube_program_);
  SAFE_DELETE(cube_);
}

#pragma mark - GLKView and GLKViewController delegate methods

const int kMinComplexity = 1;
const int kMaxComplexity = 100;

- (void)update {
  // Compute animation
  double now = Time();
  // Update camera position
  double angle = now;
  float r = 2.0f;
  camera_.set_up(Vector3f(0.0f, 1.0f, 0.0f));
  camera_.set_position(Vector3f(sin(angle)*r, r*0.75f, cos(angle)*r));
  camera_.set_look_at(Vector3f(0.0f, 0.0f, 0.0f));
  camera_.set_near_far(r * 0.1f, r * 2.0f);
}

- (void)drawScene {
  // int complexity = int(lerp(kMinComplexity, complexity_.value, kMaxComplexity));
  cube_program_->Use();
  glUniform4f(cube_uni_color_, 1.0f, 1.0f, 1.0f, 1.0f);
  Vector3f offset[] = {
    Vector3f(-0.5f, 0.25f, 0.0f),
    Vector3f(-0.5f, -0.75f, -1.0f),
    Vector3f(-1.5f, -0.75f, 0.0f)
  };
  int num = sizeof(offset) / sizeof(offset[0]);
  for (int i = 0; i < num; ++i) {
    Transform t(Matrix44f(camera_.viewprojection()));
    t.glTranslate(offset[i]);
    glUniformMatrix4fv(cube_uni_mvp_, 1, 0, t.raw());
    cube_->Draw();
  }
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
  float w = view.bounds.size.width;
  float h = view.bounds.size.height;
  camera_.SetVirtualViewport(w, h, w, h, 1);
  glClearColor(0.65f, 0.65f, 0.65f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
