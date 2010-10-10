/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Implements chord.h
 */

#include <math.h>
#include "chord.h"

void chord_init(GSequence *network)
{
  gint ns = g_sequence_get_length(network);
  gint idx = 0, jdx = 0, log_N = (gint) log2(V_SPACE_T_MAX);
  for(; idx < ns; idx++) {
    GSequenceIter *citer = g_sequence_get_iter_at_pos(network, idx);
    vertex *cv = g_sequence_get(citer);
    vertex_add_edge_by_index(network, cv, circle_left_idx(ns, idx, 1));
    for(jdx = 0; jdx <= log_N; jdx++) {
      v_space_t rid = cv->id + (1 << jdx);
      vertex *nv = vertex_next(network, rid);
      vertex_add_edge(cv, nv);
    }
  }
}

vertex *chord_greedy_route(struct message *msg, GSequence *table)
{
  return edge_predecessor(table, msg->dst)->remote;
}
