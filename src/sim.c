/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Main simulation component, edit this to perform tests.
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "greedy.h"
#include "message.h"
#include "symphony.h"

void symphony_init(struct sorted_list *network);

int route(struct message *msg, struct vertex *src)
{
  struct vertex *next = greedy_route(msg, src->table);
  struct vertex *prev = src;
  int hops = 0;
  printf("Message:  src: %d, dst: %d\n", msg->src, msg->dst);
  while(next != prev) {
    prev = next;
    next = greedy_route(msg, next->table);
    if(next == prev) {
      break;
    }
    hops++;
    printf("Hop: %d, From: %d, To: %d\n", hops, prev->id, next->id);
  }
  return hops--;
}

int main(int argc, char ** argv)
{
  int network_size = 10000;
  struct sorted_list *network = network_init(network_size);
  int idx = 0;
  for(; idx < network_size; idx++) {
    vertex_init(network, rand());
  }
  printf("Done initializing vertexes!\n");

  symphony_init(network);
//  chord_init(network, 1);
  printf("Done initializing edges!\n");

  int src_idx = network->size >> 8;
  int dst_idx = network->size >> 1;
  struct vertex *src = (struct vertex*) network->entries[src_idx];
  struct vertex *dst = (struct vertex*) network->entries[dst_idx];

  struct message msg = { src->id, dst->id };
  printf("Hops - %d\n", route(&msg, src));

  printf("Done setting up network...\n");
  system("sleep 100");
  return 0;
}
