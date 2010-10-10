/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Implements standard Chord style overlay.
 */

#ifndef CHORD_H
#define CHORD_H
#include <glib.h>
#include "graph.h"
#include "message.h"

/**
 * Initialize an unconnected network to match standard chord parameters.
 * @param network the unconnected network
 */
void chord_init(GSequence *network);
/**
 * Implements symphony based greedy routing.
 * @param msg the message to transmit.
 * @param table the current hops routing table.
 */
vertex *chord_greedy_route(struct message *msg, GSequence *table);
#endif
