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
#include <glib.h>
#include "graph.h"
#include "message.h"

void bidir_symphony_init(GSequence *network, GRand *rand, guint near, guint far);
/**
 * Implements symphony bidirectional greedy routing.
 * @param msg the message to transmit.
 * @param table the current hops routing table.
 */
vertex *bidir_symphony_greedy_route(struct message *msg, GSequence *table);

void unidir_symphony_init(GSequence *network, GRand *rand, guint near, guint far);
/**
 * Implements symphony unidirectional greedy routing.
 * @param msg the message to transmit.
 * @param table the current hops routing table.
 */
vertex *unidir_symphony_greedy_route(struct message *msg, GSequence *table);
#endif
