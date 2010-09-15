#include <glib.h>
#include "../graph.h"

/** Test basic network functions */
void test_network_init()
{
  GSequence *network = network_init();
  network_free(network);
}

/** Test basic vertex functions */
void test_vertex_init()
{
  gint idx;
  GRand* grand = g_rand_new_with_seed(g_test_rand_int());
  gint size = g_test_rand_int_range(5000, 10000);
  GSequence *network = network_init();

  for(idx = 0; idx < size; idx++) {
    v_space_t vid = v_space_rand(grand);
    vertex *v = vertex_init(vid);
    network_add_vertex(network, v);
  }

  GSequenceIter *current = g_sequence_get_begin_iter(network);
  GSequenceIter *next = g_sequence_iter_next(current);
  GSequenceIter *end = g_sequence_get_end_iter(network);

  /* Make sure they are in order... */
  g_assert_cmpuint(size, ==, g_sequence_get_length(network));
  idx = 0;
  while(next != end) {
    vertex *cv = g_sequence_get(current);
    vertex *nv = g_sequence_get(next);
    g_assert_cmpuint(cv->id, <, nv->id);
    current = next;
    next = g_sequence_iter_next(next);
    idx++;
  }
  g_assert_cmpuint(size - 1, ==, idx);

  current = g_sequence_get_iter_at_pos(network, 100);
  vertex *cv = g_sequence_get(current);
  vertex *nv = vertex_nearest(network, cv->id);
  /* ensure vertex searches work */
  g_assert_cmpuint(cv->id, ==, nv->id);
  vertex *v = vertex_init(cv->id);
  /* can't add duplicate vertice */
  g_assert(network_add_vertex(network, v) == -1);

  network_free(network);
}

/** Test graph comparators */
void test_graph_compare()
{
  vertex *v = vertex_init(0);
  g_assert(vertex_compare(v, v, NULL) == 0);
  g_assert(vertex_compare(v, NULL, &(v->id)) == 0);
  g_assert(vertex_compare(NULL, v, &(v->id)) == 0);

  GSequenceIter *current = g_sequence_get_iter_at_pos(v->table, 0);
  edge *e = g_sequence_get(current);
  g_assert(edge_compare(e, e, NULL) == 0);
  g_assert(edge_compare(NULL, e, &(e->remote->id)) == 0);
  g_assert(edge_compare(e, NULL, &(e->remote->id)) == 0);

  g_assert(edge_compare(NULL, e, &(v->id)) == 0);
  g_assert(edge_compare(e, NULL, &(v->id)) == 0);

  current = g_sequence_search(v->table, NULL,
      (int (*)(gconstpointer, gconstpointer, gpointer)) edge_compare,
      &(v->id));
  e = g_sequence_get(g_sequence_iter_prev(current));
  g_assert(v == e->remote);
}

GSequence *generate_network(GRand *grand, gint size)
{
  gint idx;
  GSequenceIter *current;
  vertex *v0, *v1;
  GSequence *network = network_init();

  for(idx = 0; idx < size; idx++) {
    v_space_t vid = v_space_rand(grand);
    v0 = vertex_init(vid);
    network_add_vertex(network, v0);
  }

  for(idx = size - 1; idx >= 0; idx--) {
    current = g_sequence_get_iter_at_pos(network, idx);
    v0 = g_sequence_get(current);
    int jdx;
    for(jdx = 0; jdx < size; jdx++) {
      current = g_sequence_get_iter_at_pos(network, jdx);
      v1 = g_sequence_get(current);
      vertex_add_edge(v0, v1);
    }
  }

  return network;
}

/** Test basic edge functions */
void test_vertex_add_edge()
{
  gint idx, size = 500;
  GSequenceIter *current;
  vertex *v0, *v1;
  edge *e;

  GRand* grand = g_rand_new_with_seed(g_test_rand_int());
  GSequence *network = generate_network(grand, size);

  current = g_sequence_get_iter_at_pos(network, 300);
  v0 = g_sequence_get(current);
  current = g_sequence_get_iter_at_pos(network, 400);
  v1 = g_sequence_get(current);
  current = g_sequence_search(v0->table, NULL,
      (int (*)(gconstpointer, gconstpointer, gpointer)) edge_compare,
      &(v1->id));
  e = g_sequence_get(g_sequence_iter_prev(current));
  g_assert(v1 == e->remote);

  for(idx = 0; idx < size; idx++) {
    current = g_sequence_get_iter_at_pos(network, idx);
    v0 = g_sequence_get(current);
    int jdx;
    for(jdx = 0; jdx < size; jdx++) {
      current = g_sequence_get_iter_at_pos(network, jdx);
      v1 = g_sequence_get(current);
      g_assert(vertex_add_edge(v0, v1) == NULL);
    }
  }

  for(idx = 0; idx < size; idx++) {
    current = g_sequence_get_iter_at_pos(network, idx);
    v0 = g_sequence_get(current);
    g_assert_cmpuint(size, ==, g_sequence_get_length(v0->table));
    int jdx;
    for(jdx = 0; jdx < size; jdx++) {
      current = g_sequence_get_iter_at_pos(network, jdx);
      v1 = g_sequence_get(current);
      current = g_sequence_search(v0->table, NULL,
          (int (*)(gconstpointer, gconstpointer, gpointer)) edge_compare,
          &(v1->id));
      e = g_sequence_get(g_sequence_iter_prev(current));
      g_assert(v1 == e->remote);
    }
  }
}

/**
 * Given a vertex and some other stuff, find the closest vertex.
 * If given another vertex move right or left half of the abs distance between
 * the two and add the delta.
 */
void help_vertex_nearest(GSequence *network, gint idx0, gint idx1,
    gboolean right, gint delta, gint equals)
{
  GSequenceIter *current = g_sequence_get_iter_at_pos(network, idx0);
  vertex *center = g_sequence_get(current);
  vertex *offset = NULL;
  v_space_t dist = center->id + delta;

  if(idx1 != -1) {
    current = g_sequence_get_iter_at_pos(network, idx1);
    offset = g_sequence_get(current);
    v_space_t tdist = v_space_abs_dist(center->id, offset->id) / 2;
    if(right == TRUE) {
      dist = circle_right_idx(V_SPACE_T_MAX, dist, tdist);
    } else {
      dist = circle_left_idx(V_SPACE_T_MAX, dist, tdist);
    }
  }

  vertex *nearest = vertex_nearest(network, dist);
  if(equals == idx0) {
    g_assert(nearest == center);
  } else {
    g_assert(nearest == offset);
  }
}

/** Test some more advanced vertex stuff */
void test_vertex_nearest()
{
  gint idx, size = 500;
  v_space_t dist;
  GRand* grand = g_rand_new_with_seed(g_test_rand_int());
  GSequence *network = generate_network(grand, size);

  help_vertex_nearest(network, 200, -1, FALSE, 0, 200);
  help_vertex_nearest(network, 200, -1, FALSE, 2, 200);
  help_vertex_nearest(network, 200, -1, FALSE, -2, 200);
  help_vertex_nearest(network, 200, 199, FALSE, 2, 200);
  help_vertex_nearest(network, 200, 199, FALSE, -2, 199);
  help_vertex_nearest(network, 200, 201, TRUE, -2, 200);
  help_vertex_nearest(network, 200, 201, TRUE, 2, 201);

  help_vertex_nearest(network, 499, -1, FALSE, 0, 499);
  help_vertex_nearest(network, 499, -1, FALSE, 2, 499);
  help_vertex_nearest(network, 499, -1, FALSE, -2, 499);
  help_vertex_nearest(network, 499, 498, FALSE, 2, 499);
  help_vertex_nearest(network, 499, 498, FALSE, -2, 498);
  help_vertex_nearest(network, 499, 0, TRUE, -2, 499);
  help_vertex_nearest(network, 499, 0, TRUE, 2, 0);

  help_vertex_nearest(network, 0, -1, 0, 0, 0);
  help_vertex_nearest(network, 0, -1, 0, 2, 0);
  help_vertex_nearest(network, 0, -1, 0, -2, 0);
  help_vertex_nearest(network, 0, 499, -1, 4, 0);
  help_vertex_nearest(network, 0, 499, -1, -2, 499);
  help_vertex_nearest(network, 0, 1, 1, -2, 0);
  help_vertex_nearest(network, 0, 1, 1, 2, 1);
}

/**
 * Given an edge and some other stuff, find the closest edge.
 * If given another edge move right or left half of the abs distance between
 * the two and add the delta.
 */
void help_edge_nearest(GSequence *table, gint idx0, gint idx1,
    gboolean right, gint delta, gint equals)
{
  GSequenceIter *current = g_sequence_get_iter_at_pos(table, idx0);
  edge *ce = g_sequence_get(current);
  edge *oe = NULL;
  vertex *center = ce->remote, *offset = NULL;
  v_space_t dist = center->id + delta;

  if(idx1 != -1) {
    current = g_sequence_get_iter_at_pos(table, idx1);
    oe = g_sequence_get(current);
    offset = oe->remote;
    v_space_t tdist = v_space_abs_dist(center->id, offset->id) / 2;
    if(right == TRUE) {
      dist = circle_right_idx(V_SPACE_T_MAX, dist, tdist);
    } else {
      dist = circle_left_idx(V_SPACE_T_MAX, dist, tdist);
    }
  }

  edge *ne = edge_nearest(table, dist);
  vertex *nearest = ne->remote;
  if(equals == idx0) {
    g_assert(nearest == center);
  } else {
    g_assert(nearest == offset);
  }
}

/** Test some more advanced edge stuff */
void test_edge_nearest()
{
  gint idx, size = 500;
  v_space_t dist;
  GRand* grand = g_rand_new_with_seed(g_test_rand_int());
  GSequence *network = generate_network(grand, size);
  GSequenceIter *current = g_sequence_get_iter_at_pos(network, 200);
  vertex *v = g_sequence_get(current);

  help_edge_nearest(v->table, 200, -1, FALSE, 0, 200);
  help_edge_nearest(v->table, 200, -1, FALSE, 2, 200);
  help_edge_nearest(v->table, 200, -1, FALSE, -2, 200);
  help_edge_nearest(v->table, 200, 199, FALSE, 2, 200);
  help_edge_nearest(v->table, 200, 199, FALSE, -2, 199);
  help_edge_nearest(v->table, 200, 201, TRUE, -2, 200);
  help_edge_nearest(v->table, 200, 201, TRUE, 2, 201);

  help_edge_nearest(v->table, 499, -1, FALSE, 0, 499);
  help_edge_nearest(v->table, 499, -1, FALSE, 2, 499);
  help_edge_nearest(v->table, 499, -1, FALSE, -2, 499);
  help_edge_nearest(v->table, 499, 498, FALSE, 2, 499);
  help_edge_nearest(v->table, 499, 498, FALSE, -2, 498);
  help_edge_nearest(v->table, 499, 0, TRUE, -2, 499);
  help_edge_nearest(v->table, 499, 0, TRUE, 2, 0);

  help_edge_nearest(v->table, 0, -1, 0, 0, 0);
  help_edge_nearest(v->table, 0, -1, 0, 2, 0);
  help_edge_nearest(v->table, 0, -1, 0, -2, 0);
  help_edge_nearest(v->table, 0, 499, -1, 4, 0);
  help_edge_nearest(v->table, 0, 499, -1, -2, 499);
  help_edge_nearest(v->table, 0, 1, 1, -2, 0);
  help_edge_nearest(v->table, 0, 1, 1, 2, 1);
}

void test_graph()
{
  g_test_add_func("/graph/network_init", test_network_init);
  g_test_add_func("/graph/vertex_init", test_vertex_init);
  g_test_add_func("/graph/comparison", test_graph_compare);
  g_test_add_func("/graph/vertex_add_edge", test_vertex_add_edge);
  g_test_add_func("/graph/vertex_nearest", test_vertex_nearest);
  g_test_add_func("/graph/edge_nearest", test_edge_nearest);
}
