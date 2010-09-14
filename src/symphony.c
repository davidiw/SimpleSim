/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Implements symphony.h
 */

#include <math.h>
#include <stdlib.h>
#include "symphony.h"

void symphony_init(struct sorted_list *network)
{
  int idx = 0, ns = network->size;
  double log_ns = log2(ns), log_N = log2(V_SPACE_T_MAX);
  for(; idx < ns; idx++) {
    int jdx = 0;
    struct vertex *current = (struct vertex *) network->entries[idx];
    vertex_add_edge(network->entries[idx], network->entries[circle_left_idx(ns, idx, 1)]);
    vertex_add_edge(network->entries[idx], network->entries[circle_left_idx(ns, idx, 2)]);
    vertex_add_edge(network->entries[idx], network->entries[circle_right_idx(ns, idx, 1)]);
    vertex_add_edge(network->entries[idx], network->entries[circle_right_idx(ns, idx, 2)]);

    while(jdx < log_ns) {
      v_space_t rid = (v_space_t) exp2(log_N - (1 - drand48()) * log_ns) + current->id;
      int nearest_idx = sorted_list_get_index(network, &rid);
      if(nearest_idx < 0) {
        nearest_idx = ~nearest_idx;
      }

      if(nearest_idx == ns) {
        struct vertex *vend = (struct vertex *) network->entries[nearest_idx - 1];
        struct vertex *vstart = (struct vertex *) network->entries[0];
        v_space_t end = v_space_abs_dist(rid, vend->id);
        v_space_t start = v_space_abs_dist(rid, vstart->id);
        if(v_space_compare(end, start) < 0) {
          nearest_idx = nearest_idx - 1;
        } else {
          nearest_idx = 0;
        }
      }

      if(vertex_add_edge(network->entries[idx], network->entries[nearest_idx]) != NULL) {
        jdx++;
      }
    }
  }
}

void symphony_add_vertex(struct vertex **network, struct vertex *vertex)
{
}
