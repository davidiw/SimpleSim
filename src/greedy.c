/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Implements greedy.h
 */

#include "greedy.h"

struct vertex *greedy_route(struct message *msg, struct edge *table)
{
  struct edge* ce = table;
  struct edge* last = table;

  struct edge* next = table;
  v_space_t cdist = v_space_abs_dist(msg->dst, ce->remote->id);

  while((ce = ce->next) != last) {
    int dist = v_space_abs_dist(msg->dst, ce->remote->id);
    if(dist < cdist) {
      cdist = dist;
      next = ce;
    }
  }

  return next->remote;
}
