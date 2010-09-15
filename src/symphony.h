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

void symphony_init(GSequence *network, GRand *rand, guint near, guint far);
#endif
