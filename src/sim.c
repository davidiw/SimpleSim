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
#include "sim_helper.h"
#include "graph.h"
#include "greedy.h"
#include "message.h"

int route(struct message *msg, struct vertex *src)
{
  struct vertex *next = greedy_route(msg, src->table);
  struct vertex *lnext = NULL;
  int hops = 0;
  printf("Message:  src: %d, dst: %d\n", msg->src, msg->dst);
  while(next != lnext) {
    hops++;
    lnext = next;
    next = greedy_route(msg, next->table);
    printf("lnext - %d, next - %d\n", lnext->id, next->id);
  }
  return hops;
}

int main(int argc, char ** argv)
{
  int network_size = 1000;
  struct sorted_list *network = network_init(network_size);

  v_space_t idx = 0;
  for(; idx < network_size; idx++) {
    vertex_init(network, idx);
  }
  printf("Done initializing vertexes!\n");

  chord_init(network, 1);
  printf("Done initializing edges!\n");


  struct message msg = { 11, network_size >> 3 };
  printf("Hops - %d\n", route(&msg, network->entries[11]));

  printf("Done setting up network...\n");
  system("sleep 100");

  return 0;
}
