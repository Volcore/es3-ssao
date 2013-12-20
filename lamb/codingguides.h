/*******************************************************************************
    Copyright (c) 2010, Limbic Software, Inc.
    All rights reserved.
 ******************************************************************************/
#ifndef LAMB_CODINGGUIDES_H_
#define LAMB_CODINGGUIDES_H_

#include <stdint.h>
#include "lamb/log/log.h"

#define DISALLOW_ASSIGN(TypeName) \
    void operator=(const TypeName&)

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);               \
    void operator=(const TypeName&)

#define SAFE_FREE(x) \
    if (x != 0) { \
      free(x); \
      x = 0; \
    }

#define SAFE_DELETE(x) \
    if (x != 0) { \
      delete x; \
      x = 0; \
    }

#define SAFE_DELETE_ARRAY(x) \
    if (x != 0) { \
      delete [] x; \
      x = 0; \
    }

#define EMPTY_STD_VECTOR(x) \
    while (x.empty() == false) {\
      delete x.back(); \
      x.pop_back(); \
    }

#define EMPTY_STD_LIST(x) \
    while (x.empty() == false) {\
      delete x.back(); \
      x.pop_back(); \
    }

template<class c> void EmptySTDMap(c *map) {
  typename c::const_iterator it = map->begin();
  while (it != map->end()) {
    if (it->second != 0) {
      delete it->second;
    }
    ++it;
  }
  map->clear();
}

#endif  // LAMB_CODINGGUIDES_H_
