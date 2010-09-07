/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Implements standard Chord style overlay.
 */

#ifndef SIMPLE_H
#define SIMPLE_H
#include "graph.h"
#include "sorted_list.h"

/**
 * Initialize an unconnected network to match standard chord parameters.
 * @param network the unconnected network
 */
void chord_init(struct sorted_list *network);
void chord_add_vertex(struct sorted_list *network, struct vertex *vertex);
#endif
