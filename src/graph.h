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
#include <glib.h>

/** Place all node identifiers in the unsigned integers space.
 * There's no need to support more than 2^64 nodes or even 2^32, so this will
 * provide for reasonable uniform distribution for the largest networks we can
 * generate for simulation purposes.
 */
typedef guint32 v_space_t;
#define V_SPACE_T_MAX G_MAXUINT32

/** Represents a single point in the overlay.  */
typedef struct _vertex {
  /** ID for the vertex */
  v_space_t id;
  /** Table of vertices from this node */
  GSequence *table;
} vertex;

/** Represents a connection to a remote vertex. */
typedef struct _edge {
  /** The remote vertex. */
  vertex *remote;
} edge;

GSequence *network_init();
void network_free(GSequence *network);
int network_add_vertex(GSequence *network, vertex *v);

/**
 * Creates a unidirectional edge from local to remote and stores it in the
 * routing table of the local node.
 * @param local Local vertex (one adding a new link)
 * @param remote The destination of the new edge
 */
edge *vertex_add_edge(vertex *local, vertex *remote);

edge *vertex_add_edge_by_index(GSequence *network, vertex *local, gint idx);
int vertex_compare(const vertex *v0, const vertex *v1, v_space_t *other);
void vertex_free(vertex *v);

/**
 * Create a new vertex with the specified ID and a routing table with an entry
 * pointing to itself.
 * @param id Unique identifier for the vertex.
 */
vertex *vertex_init(v_space_t id);

/**
 * Returns the vertex closest in space to the target.
 * @param seq A sequence of vertex.
 * @param target The id to find.
 */
vertex *vertex_nearest(GSequence *seq, v_space_t target);

/**
 * Returns the vertex immediately after the target.
 * @param seq A sequence of vertex.
 * @param target The id to find.
 */
vertex *vertex_next(GSequence *seq, v_space_t target);

int edge_compare(const edge *e0, const edge *e1, v_space_t *other);
void edge_free(edge *v);
/**
 * Returns the edge closest in space to the target.
 * @param seq A sequence of edges.
 * @param target The id to find.
 */
edge *edge_nearest(GSequence *seq, v_space_t);
edge *edge_predecessor(GSequence *seq, v_space_t target);

inline v_space_t circle_right_idx(v_space_t network_size, v_space_t idx, v_space_t count);
inline v_space_t circle_left_idx(v_space_t network_size, v_space_t idx, v_space_t count);
v_space_t v_space_abs_dist(v_space_t v0, v_space_t v1);
int v_space_compare(v_space_t v0, v_space_t v1);
v_space_t v_space_rand(GRand *grand);
#endif
