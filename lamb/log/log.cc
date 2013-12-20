/*******************************************************************************
    Copyright (c) 2012, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#include "lamb/log/log.h"
#include <stdio.h>
#include <stdarg.h>
#include <string>


void LogNoDate(const char *const text, ...) {
  va_list args;
  va_start(args, text);
  vprintf(text, args);
  va_end(args);
}

void Log(const char *const text, ...) {
  va_list args;
  va_start(args, text);
  vprintf(text, args);
  va_end(args);
}
