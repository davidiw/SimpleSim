/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Implements a Symphony style overlay.
 */

#ifndef SYMPHONY_H
#define SYMPHONY_H
#include "graph.h"

void symphony_init(struct vertex **network, int near_edges);
void symphony_add_vertex(struct vertex **network, struct vertex *vertex);
#endif
