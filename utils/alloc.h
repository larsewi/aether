#ifndef _AETHER_ALLOC_H
#define _AETHER_ALLOC_H

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"

inline void *xmalloc(const size_t size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    LOG_CRITICAL("Failed to allocate memory: %s", strerror(errno));
  }
  return ptr;
}

#endif // _AETHER_ALLOC_H
