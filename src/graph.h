/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Provides common functions for interaction with vertexes (the overlay).
 */

#ifndef GRAPH_H
#define GRAPH_H
#include <limits.h>
#include "sorted_list.h"

/** Place all node identifiers in the unsigned integers space.
 * There's no need to support more than 4 billion nodes and this can contain a
 * reasonable uniform distribution for at least 10 to if not millions of nodes.
 */
typedef unsigned int v_space_t;
#define V_SPACE_T_MAX UINT_MAX

/** Represents a connection to a remote vertex. */
struct edge {
  /** The remote vertex. */
  struct vertex *remote;
  /** The next connection in the linked-list. */
  struct edge *next;
};

/** Represents a single point in the overlay.  */
struct vertex {
  /** ID for the vertex */
  v_space_t id;
  /** Pointer to the connected remote vertex with the lowest id. */
  struct edge *table;
  /** Pointer to the connected remote vertex with the highest id. */
  struct edge *end_of_table;
};

/**
 * Create a new vertex.
 * Creates a new vertex with the specified ID and a routing table pointing to
 * itself, routing tables are never null.
 * @param id Unique identifier for the vertex.
 */
struct vertex *vertex_init(struct sorted_list *network, v_space_t id);

/**
 * Creates a unidirectional edge from local to remote.
 * The edge is stored in the local vertex's routing table from smallest id to
 * largest id.
 * @param local Local vertex (one adding a new link)
 * @param remote The destination of the new edge
 */
struct edge *vertex_add_edge(struct vertex *local, struct vertex *remote);

int vertex_compare(struct vertex *v0, struct vertex *v1);
int vertex_compare_by_identifier(struct vertex *v, v_space_t *id);
void vertex_free(struct vertex *v);
struct sorted_list *network_init(int network_size);

inline v_space_t circle_right_idx(v_space_t network_size, v_space_t idx, v_space_t count);
inline v_space_t circle_left_idx(v_space_t network_size, v_space_t idx, v_space_t count);
v_space_t v_space_abs_dist(v_space_t v0, v_space_t v1);
int v_space_compare(v_space_t v0, v_space_t v1);
#endif
