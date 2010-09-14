/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Implements chord.h
 */

#include <limits.h>
#include <stdlib.h>
#include "chord.h"

void chord_init(struct sorted_list *network)
{
  int idx = 0, jdx = 0;
  int ns = network->size;
  for(; idx < ns; idx++) {
    /* Add a connection to the peer immediately before this node */
    vertex_add_edge(network->entries[idx], network->entries[circle_left_idx(ns, idx, 1)]);

    /* Add the 31 nodes to the right of the current node indexed by 2^i
     * distance from this node.*/
    v_space_t right_dist = 1;
    v_space_t id = ((struct vertex *)network->entries[idx])->id;
    while(right_dist != 0) {
      v_space_t abs_dist = circle_right_idx(UINT_MAX, id, right_dist);
      int jdx = sorted_list_get_index(network, &abs_dist);

      if(jdx < 0) {
        jdx = ~jdx;
      }

      if(jdx >= network->size) {
        jdx = network->size - 1;
      }

      if(vertex_add_edge(network->entries[idx], network->entries[jdx]) == NULL) {
        /** @todo It has been a while since I reviewed the paper, so for now,
         * the connection is made to the next closest node for which there exists
         * no connection.
         */
        int iter = 0;
        for(; iter < network->size; iter++) {
          jdx = circle_right_idx(network->size, jdx, 1);
          if(vertex_add_edge(network->entries[idx], network->entries[jdx]) != NULL) {
            break;
          }
        }
      }
      right_dist <<= 1;
    }
  }
}

void chord_add_vertex(struct sorted_list *network, struct vertex *vertex)
{
}
