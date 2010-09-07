/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * A standard greedy routing algorithm.
 */
#ifndef GREEDY_H
#define GREEDY_H
#include "graph.h"
#include "message.h"

/**
 * Implements traditional structured overlay greedy routing.
 * @param msg the message to transmit.
 * @param table the current hops routing table.
 */
struct vertex *greedy_route(struct message *msg, struct edge* table);
#endif
