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
#include "sim_helper.h"
#include "sorted_list.h"

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
inline int circle_right_idx(v_space_t network_size, v_space_t idx, v_space_t count);
inline int circle_left_idx(v_space_t network_size, v_space_t idx, v_space_t count);
#endif
