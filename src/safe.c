/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Implements safe.h
 */

#include "safe.h"

inline void *Malloc(size_t size)
{
  void *mem = malloc(size);
  if(mem == NULL) {
    perror("Out of memory");
    exit(-1);
  }
  memset(mem, 0, size);
  return mem;
}
