/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Implements symphony.h
 */

#include <stdlib.h>
#include <math.h>
#include "symphony.h"

void symphony_init(GSequence *network, GRand *rand, guint near, guint far, gboolean unidir)
{
  gint idx = 0, ns = g_sequence_get_length(network);
  gdouble log_ns = log2(ns), log_N = log2(V_SPACE_T_MAX);

  for(; idx < ns; idx++) {
    GSequenceIter *citer = g_sequence_get_iter_at_pos(network, idx);
    vertex *cv = g_sequence_get(citer);

    guint jdx;
    for(jdx = 1; jdx <= near; jdx++) {
      vertex_add_edge_by_index(network, cv, circle_left_idx(ns, idx, jdx));
      vertex_add_edge_by_index(network, cv, circle_right_idx(ns, idx, jdx));
    }

    jdx = 0;
    while(jdx < far) {
      gdouble double_idx = log_N - (1.0 - g_rand_double(rand)) * log_ns;
      v_space_t rid = circle_right_idx(V_SPACE_T_MAX, cv->id, ((v_space_t) exp2(double_idx)));

      vertex *nv = vertex_nearest(network, rid);
      if(vertex_add_edge(cv, nv) != NULL) {
        if(unidir == FALSE) {
          vertex_add_edge(nv, cv);
        }
        jdx++;
      }
    }
  }
}

void bidir_symphony_init(GSequence *network, GRand *rand, guint near, guint far)
{
  return symphony_init(network, rand, near, far, FALSE);
}

vertex *bidir_symphony_greedy_route(struct message *msg, GSequence *table)
{
  return edge_nearest(table, msg->dst)->remote;
}

void unidir_symphony_init(GSequence *network, GRand *rand, guint near, guint far)
{
  return symphony_init(network, rand, near, far, TRUE);
}

vertex *unidir_symphony_greedy_route(struct message *msg, GSequence *table)
{
  return edge_predecessor(table, msg->dst)->remote;
}
