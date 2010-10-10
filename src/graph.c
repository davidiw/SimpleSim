/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Implements graph.h
 */

#include "graph.h"

/* network functions */

GSequence *network_init()
{
  return g_sequence_new((void (*)(gpointer)) vertex_free);
}

void network_free(GSequence *network)
{
  g_sequence_free(network);
}

int network_add_vertex(GSequence *network, vertex *v)
{
  vertex *nv = vertex_nearest(network, v->id);
  if(nv != NULL && v->id == nv->id) {
    return -1;
  }
  g_sequence_insert_sorted(network, v,
      (int (*)(gconstpointer, gconstpointer, gpointer)) vertex_compare,
      NULL);
}

/* vertex functions */

edge *vertex_add_edge(vertex *local, vertex *remote)
{
  GSequenceIter *iter = g_sequence_search(local->table, NULL,
      (int (*)(gconstpointer, gconstpointer, gpointer)) edge_compare,
      &(remote->id));

  if(g_sequence_iter_is_begin(iter) == FALSE) {
    edge *e = g_sequence_get(g_sequence_iter_prev(iter));
    if(e->remote == remote) {
      return NULL;
    }
  }

  edge *e = g_slice_alloc0(sizeof(edge));
  e->remote = remote;
  g_sequence_insert_sorted(local->table, e,
      (int (*)(gconstpointer, gconstpointer, gpointer)) edge_compare, NULL);
  return e;
}

edge *vertex_add_edge_by_index(GSequence *network, vertex *local, gint idx)
{
  GSequenceIter *iter = g_sequence_get_iter_at_pos(network, idx);
  return vertex_add_edge(local, g_sequence_get(iter));
}

int vertex_compare(const vertex *v0, const vertex *v1, v_space_t *other)
{
  if(v0 == NULL) {
    return v_space_compare(*other, v1->id);
  } else if(v1 == NULL) {
    return v_space_compare(v0->id, *other);
  } else {
    return v_space_compare(v0->id, v1->id);
  }
}

void vertex_free(vertex *v)
{
  if(v->table == NULL) {
    perror("Vertex table already freed!");
  } else {
    g_sequence_free(v->table);
  }
  g_slice_free1(sizeof(vertex), v);
}

vertex *vertex_init(v_space_t id)
{
  vertex *v = g_slice_alloc0(sizeof(vertex));
  v->id = id;
  v->table = g_sequence_new((void (*)(gpointer)) edge_free);
  vertex_add_edge(v, v);
  return v;
}

vertex *vertex_nearest(GSequence *seq, v_space_t target)
{
  GSequenceIter *idx0 = g_sequence_search(seq, NULL,
      (int (*)(gconstpointer, gconstpointer, gpointer)) vertex_compare,
      &(target));
  GSequenceIter *idx1;

  if(g_sequence_iter_is_begin(idx0) == TRUE) {
    idx1 = g_sequence_get_end_iter(seq);
    if(idx0 == idx1) {
      return NULL;
    }
    idx1 = g_sequence_iter_prev(idx1);
  } else if(g_sequence_iter_is_end(idx0) == TRUE) {
    idx0 = g_sequence_iter_prev(idx0);
    idx1 = g_sequence_get_begin_iter(seq);
  } else {
    idx1 = g_sequence_iter_prev(idx0);
  }

  vertex *v0 = g_sequence_get(idx0);
  if(g_sequence_iter_is_end(idx1) == TRUE) {
    return v0;
  }
  vertex *v1 = g_sequence_get(idx1);

  v_space_t dist0 = v_space_abs_dist(target, v0->id);
  v_space_t dist1 = v_space_abs_dist(target, v1->id);
  if(v_space_compare(dist0, dist1) < 0) {
    return v0;
  }
  return v1;
}

vertex *vertex_next(GSequence *seq, v_space_t target)
{
  GSequenceIter *idx = g_sequence_search(seq, NULL,
      (int (*)(gconstpointer, gconstpointer, gpointer)) vertex_compare,
      &(target));

  if(g_sequence_iter_is_end(idx) == TRUE) {
    idx = g_sequence_iter_prev(idx);
  }
  return (vertex *) g_sequence_get(idx);
}

/* edge functions */

int edge_compare(const edge *e0, const edge *e1, v_space_t *other)
{
  if(e0 == NULL) {
    return v_space_compare(*other, e1->remote->id);
  } else if(e1 == NULL) {
    return v_space_compare(e0->remote->id, *other);
  } else {
    return v_space_compare(e0->remote->id, e1->remote->id);
  }
}

void edge_free(edge *e)
{
  g_slice_free1(sizeof(edge), e);
}

edge *edge_nearest(GSequence *seq, v_space_t target)
{
  GSequenceIter *idx0 = g_sequence_search(seq, NULL,
      (int (*)(gconstpointer, gconstpointer, gpointer)) edge_compare,
      &(target));
  GSequenceIter *idx1;

  if(g_sequence_iter_is_begin(idx0) == TRUE) {
    idx1 = g_sequence_get_end_iter(seq);
    if(idx0 == idx1) {
      return NULL;
    }
    idx1 = g_sequence_iter_prev(idx1);
  } else if(g_sequence_iter_is_end(idx0) == TRUE) {
    idx0 = g_sequence_iter_prev(idx0);
    idx1 = g_sequence_get_begin_iter(seq);
  } else {
    idx1 = g_sequence_iter_prev(idx0);
  }

  edge *e0 = g_sequence_get(idx0);
  if(g_sequence_iter_is_end(idx1) == TRUE) {
    return e0;
  }
  edge *e1 = g_sequence_get(idx1);

  v_space_t dist0 = v_space_abs_dist(target, e0->remote->id);
  v_space_t dist1 = v_space_abs_dist(target, e1->remote->id);
  if(v_space_compare(dist0, dist1) < 0) {
    return e0;
  }
  return e1;
}

edge *edge_predecessor(GSequence *seq, v_space_t target)
{
  GSequenceIter *idx = g_sequence_search(seq, NULL,
      (int (*)(gconstpointer, gconstpointer, gpointer)) edge_compare,
      &(target));

  if(g_sequence_iter_is_begin(idx) == TRUE) {
    idx = g_sequence_get_end_iter(seq);
  }
  idx = g_sequence_iter_prev(idx);
  return (edge *)  g_sequence_get(idx);
}

/* v_space_t functions */

v_space_t circle_left_idx(v_space_t network_size, v_space_t idx, v_space_t count)
{
  if(idx < count) {
    return network_size + idx - count;
  } else {
    return idx - count;
  }
}

v_space_t circle_right_idx(v_space_t network_size, v_space_t idx, v_space_t count)
{
  return (idx + count) % network_size;
}

v_space_t v_space_abs_dist(v_space_t v0, v_space_t v1)
{
  v_space_t near, around;

  if(v0 < v1) {
    near = v1 - v0;
    around = (V_SPACE_T_MAX - v1) + v0;
  } else if(v1 < v0) {
    near = v0 - v1;
    around = (V_SPACE_T_MAX - v0) + v1;
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

v_space_t v_space_rand(GRand *grand)
{
  v_space_t lower = g_rand_int(grand);
  return lower;
//  v_space_t upper = ((v_space_t) g_rand_int(grand)) << 32;
//  return upper | lower;
}
