/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Contents of messages via over the overlay.
 */

#ifndef MESSAGE_H
#define MESSAGE_H
#include "graph.h"

struct message {
  v_space_t src;
  v_space_t dst;
};
#endif
