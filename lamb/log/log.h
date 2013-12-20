/*******************************************************************************
    Copyright (c) 2012, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_LOG_LOG_H_
#define LAMB_LOG_LOG_H_

#include "lamb/codingguides.h"

#define NYI Log("Function not implemented: '%s' %s:%i\n", __func__, __FILE__, __LINE__)
#define NYI_ONCE {\
  static bool once = true;\
  if (once) {\
    once = false;\
    Log("Function not implemented: '%s' %s:%i\n", __func__, __FILE__, __LINE__);\
  }\
}

void SetLoggingToFile(bool state);
void Log(const char *const text, ...);
void LogNoDate(const char *const text, ...);

#endif  // LAMB_LOG_LOG_H_
