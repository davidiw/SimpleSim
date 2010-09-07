/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory.
 * @section DESCRIPTION
 * Provides failure checking on library calls.
 */

#ifndef _SAFE_H
#define _SAFE_H
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

/**
 * Provides a safe Malloc, that exits the program if out of memory.
 * @param size amount of memory in bytes to allocate.
 */
inline void *Malloc(size_t size);
#endif
