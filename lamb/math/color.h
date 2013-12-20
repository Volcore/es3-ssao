/*******************************************************************************
    Copyright (c) 2009, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_MATH_COLOR_H_
#define LAMB_MATH_COLOR_H_

#include "lamb/math/vector4f.h"

/** Default colors. */
#define COLOR_SHINY_SILVER      Vector4f(0xee/255.0f, 0xee/255.0f, 0xee/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_REDDIT_WHITE      Vector4f(0xff/255.0f, 0xff/255.0f, 0xff/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_MAGNOLIA          Vector4f(0xf9/255.0f, 0xf7/255.0f, 0xed/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_ACTION_YELLOW     Vector4f(0xff/255.0f, 0xff/255.0f, 0x88/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_QOOP_MINT         Vector4f(0xcd/255.0f, 0xeb/255.0f, 0x8b/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_GMAIL_BLUE        Vector4f(0xc3/255.0f, 0xd9/255.0f, 0xff/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_SHADOW_GREY       Vector4f(0x36/255.0f, 0x39/255.0f, 0x3d/255.0f, 0xff/255.0f) // NOLINT

#define COLOR_MOZILLA_RED       Vector4f(0xff/255.0f, 0x1a/255.0f, 0x00/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_ROLLYO_RED        Vector4f(0xcc/255.0f, 0x00/255.0f, 0x00/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_RSS_ORANGE        Vector4f(0xff/255.0f, 0x74/255.0f, 0x00/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_TECHCRUNCH_GREEN  Vector4f(0x00/255.0f, 0x8c/255.0f, 0x00/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_NEWSVINE_GREEN    Vector4f(0x00/255.0f, 0x6e/255.0f, 0x2e/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_FLOCK_BLUE        Vector4f(0x40/255.0f, 0x96/255.0f, 0xee/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_FLICKR_PINK       Vector4f(0xff/255.0f, 0x00/255.0f, 0x84/255.0f, 0xff/255.0f) // NOLINT

#define COLOR_ROR_RED           Vector4f(0xb0/255.0f, 0x2b/255.0f, 0x2c/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_ETSY_VERMILLION   Vector4f(0xd1/255.0f, 0x56/255.0f, 0x00/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_43_THINGS_GOLD    Vector4f(0xc7/255.0f, 0x98/255.0f, 0x10/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_WRITELY_OLIVE     Vector4f(0x73/255.0f, 0x88/255.0f, 0x0a/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_BASECAMP_GREEN    Vector4f(0x6b/255.0f, 0xba/255.0f, 0x70/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_MOZILLA_BLUE      Vector4f(0x3f/255.0f, 0x4c/255.0f, 0x6b/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_DIGG_BLUE         Vector4f(0x35/255.0f, 0x6a/255.0f, 0xa0/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_LASTFM_CRIMSON    Vector4f(0xd0/255.0f, 0x1f/255.0f, 0x3c/255.0f, 0xff/255.0f) // NOLINT

#define COLOR_YELLOW            Vector4f(0xff/255.0f, 0xff/255.0f, 0x00/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_RED               Vector4f(0xff/255.0f, 0x00/255.0f, 0x00/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_GREEN             Vector4f(0x00/255.0f, 0xff/255.0f, 0x00/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_BLUE              Vector4f(0x00/255.0f, 0x00/255.0f, 0xff/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_WHITE             Vector4f(0xff/255.0f, 0xff/255.0f, 0xff/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_BLACK             Vector4f(0x00/255.0f, 0x00/255.0f, 0x00/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_REALLYDARKGRAY    Vector4f(0x11/255.0f, 0x11/255.0f, 0x11/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_DARKGRAY          Vector4f(0x44/255.0f, 0x44/255.0f, 0x44/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_LIGHTGRAY         Vector4f(0x88/255.0f, 0x88/255.0f, 0x88/255.0f, 0xff/255.0f) // NOLINT
#define COLOR_BGGRAY            Vector4f(100.0f/255.0f, 100.0f/255.0f, 100.0f/255.0f, 0xff/255.0f) // NOLINT

#define COLOR_MENU_BLUE         Vector4f(33.0f/255.0f, 69.0f/255.0f, 146.0f/255.0f, 1.0f) // NOLINT
#define COLOR_MENU_BROWN        Vector4f(144.0f/255.0f, 117.0f/255.0f, 80.0f/255.0f, 0.7f) // NOLINT
#define COLOR_MENU_LIME         Vector4f(185.0f/255.0f, 228.0f/255.0f, 132.0f/255.0f, 1.0f) // NOLINT
#define COLOR_MENU_GREEN        Vector4f(93.0f/255.0f, 121.0f/255.0f, 63.0f/255.0f, 0.7f) // NOLINT

static inline Vector4f NormalizeColor(int r, int g, int b, int a) {
  return Vector4f(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
}

static inline Vector4f ColorForInt(int x) {
  switch (x % 6) {
  default:
  case 0: return COLOR_RED;
  case 1: return COLOR_GREEN;
  case 2: return COLOR_BLUE;
  case 3: return COLOR_ACTION_YELLOW;
  case 4: return COLOR_RSS_ORANGE;
  case 5: return COLOR_FLICKR_PINK;
  }
}

static inline Vector4f GreenColorForInt(int x) {
  switch (x % 5) {
  default:
  case 0: return COLOR_TECHCRUNCH_GREEN;
  case 1: return COLOR_QOOP_MINT;
  case 2: return COLOR_BASECAMP_GREEN;
  case 3: return COLOR_WRITELY_OLIVE;
  case 4: return COLOR_NEWSVINE_GREEN;
  }
}


#endif  // LAMB_MATH_COLOR_H_
