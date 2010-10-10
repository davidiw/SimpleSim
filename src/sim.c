/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Main simulation component, edit this to perform tests.
 */

#include <glib.h>
#include <glib/gprintf.h>
#include "graph.h"
#include "message.h"
#include "chord.h"
#include "symphony.h"

int route(struct message *msg, vertex *src, vertex *(*router)(struct message *, GSequence *))
{
  vertex *next = router(msg, src->table);
  vertex *prev = src;
  int hops = 0;
  g_printf("Message:  src: %lu, dst: %lu\n", msg->src, msg->dst);
  while(next != prev) {
    printf("%u %u\n", prev->id, next->id);
    prev = next;
    next = router(msg, next->table);
    if(next == prev) {
      break;
    }
    hops++;
  }
  return hops--;
}

static gchar *network_type = "symphony";
static gint network_size = 1000;
static gint close_links = 1;
static gint far_links = 1;
static gint seed = 0;
static GOptionEntry entries[] =
{
  { "network_size", 'n', 0, G_OPTION_ARG_INT, &network_size, "Network size", NULL },
  { "network_type", 't', 0, G_OPTION_ARG_STRING, &network_type, "Network overlay type (unidir_symphony, symphony, chord, pastry)", NULL },
  { "close_links", 'c', 0, G_OPTION_ARG_INT, &close_links, "Close links", NULL },
  { "far_links", 'f', 0, G_OPTION_ARG_INT, &far_links, "Far links", NULL },
  { "seed", 's', 0, G_OPTION_ARG_INT, &seed, "Random seed", NULL },
  { NULL }
};

int main(int argc, char ** argv)
{
  GError *error = NULL;
  GOptionContext *context = g_option_context_new("- SimpleSim, P2P Structured Overlay Simulator");
  g_option_context_add_main_entries(context, entries, NULL);
  if(!g_option_context_parse(context, &argc, &argv, &error)) {
    g_print("option parsing failed: %s\n", error->message);
    return -1;
  }

  //g_mem_set_vtable(glib_mem_profiler_table);
  GRand* grand = g_rand_new_with_seed(seed);
  GSequence *network = network_init();
  int idx = 0;
  for(; idx < network_size; idx++) {
    vertex *v = vertex_init(v_space_rand(grand));
    g_sequence_insert_sorted(network, v,
        (int (*)(gconstpointer, gconstpointer, gpointer)) vertex_compare, NULL);
  }
  g_printf("Done initializing vertexes!\n");

  vertex *(*router)(struct message *, GSequence *) = NULL;
  if(strcmp(network_type, "symphony") == 0) {
    bidir_symphony_init(network, grand, close_links, far_links);
    router = bidir_symphony_greedy_route;
  } else if(strcmp(network_type, "unidir_symphony") == 0) {
    unidir_symphony_init(network, grand, close_links, far_links);
    router = unidir_symphony_greedy_route;
  } else if(strcmp(network_type, "chord") == 0) {
    chord_init(network);
    router = chord_greedy_route;
  } else {
    g_printf("No such netowrk type: %s\n", network_type);
  }
  g_printf("Done initializing edges!\n");

  int src_idx = network_size >> 8;
  int dst_idx = network_size >> 1;

  GSequenceIter *iter = g_sequence_get_iter_at_pos(network, src_idx);
  vertex *src = g_sequence_get(iter);
  iter = g_sequence_get_iter_at_pos(network, dst_idx);
  vertex *dst = g_sequence_get(iter);

  struct message msg = { src->id, dst->id };
  g_printf("Hops - %d\n", route(&msg, src, router));

  g_printf("Done setting up network...\n");
  g_rand_free(grand);
  network_free(network);
  return 0;
}
