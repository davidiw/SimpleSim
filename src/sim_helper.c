/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Implements sim_helper.h
 */

#include <limits.h>
#include "sim_helper.h"

v_space_t v_space_abs_dist(v_space_t v0, v_space_t v1)
{
  unsigned int near, around;

  if(v0 < v1) {
    near = v1 - v0;
    around = (UINT_MAX - v1) + v0;
  } else if(v1 < v0) {
    near = v0 - v1;
    around = (UINT_MAX - v0) + v1;
  } else {
    return 0;
  }

  return near < around ? near : around;
}

int v_space_compare(v_space_t v0, v_space_t v1)
{
  if(v0 < v1) {
    return -1;
  } else if(v0 > v1) {
    return 1;
  } else {
    return 0;
  }
}
