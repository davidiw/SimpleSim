/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Implements graph.h
 */

#include "graph.h"
#include "safe.h"

void vertex_free_void(void *v);
int vertex_compare_void(void *v0, void *v1);
int vertex_compare_by_identifier_void(void *v, void *id);

struct sorted_list *network_init(int network_size)
{
  return sorted_list_init(network_size, sizeof(struct vertex *), 
      (int (*)(void *, void *)) vertex_compare,
      (int (*)(void *, void *)) vertex_compare_by_identifier,
      (void (*)(void *)) vertex_free);
}

struct vertex *vertex_init(struct sorted_list *network, v_space_t id)
{
  struct vertex *v = (struct vertex *) Malloc(sizeof (struct vertex));
  v->id = id;

  struct edge *e = Malloc(sizeof(struct edge));
  e->remote = v;
  e->next = e;

  v->table = e;
  v->end_of_table = e;

  sorted_list_add_element(network, v);
  return v;
}

void vertex_free(struct vertex *v)
{
  if(v == NULL) {
    perror("Vertex already freed!");
    return;
  }

  if(v->table == NULL) {
    perror("Vertex table already freed!");
  } else {
    struct edge *current = v->table;
    struct edge *next = current->next;

    do {
      free(current);
      current = next;
      next = current->next;
    } while(next != v->table);
  }

  free(v);
}

struct edge *vertex_add_edge(struct vertex *local, struct vertex *remote)
{
  struct edge *e = (struct edge *) Malloc(sizeof(struct edge));
  e->remote = remote;

  struct edge *ce = local->table;
  struct edge *last = ce;

  while(ce != local->end_of_table) {
    if(v_space_compare(remote->id, ce->remote->id) <= 0) {
      break;
    }

    last = ce;
    ce = ce->next;
  }

  if(ce->next->remote == remote || ce->remote == remote) {
    return NULL;
  }

  if(ce == local->table) {
    e->next = ce;
    local->table = e;
    local->end_of_table->next = local->table;
  } else if(ce == local->end_of_table) {
    e->next = local->table;
    local->end_of_table->next = e;
    local->end_of_table = e;
  } else {
    e->next = ce;
    last->next = e;
  }

  return e;
}

int vertex_compare(struct vertex *v0, struct vertex *v1)
{
  return v_space_compare(v0->id, v1->id);
}

int vertex_compare_by_identifier(struct vertex *v, v_space_t *id)
{
  return v_space_compare(v->id, *id);
}

inline v_space_t circle_left_idx(v_space_t network_size, v_space_t idx, v_space_t count)
{
  if(idx < count) {
    return network_size + idx - count;
  } else {
    return idx - count;
  }
}

inline v_space_t circle_right_idx(v_space_t network_size, v_space_t idx, v_space_t count)
{
  return (idx + count) % network_size;
}

v_space_t v_space_abs_dist(v_space_t v0, v_space_t v1)
{
  v_space_t near, around;

  if(v0 < v1) {
    near = v1 - v0;
    around = (UINT_MAX - v1) + v0;
  } else if(v1 < v0) {
    near = v0 - v1;
    around = (UINT_MAX - v0) + v1;
  } else {
    return 0;
  }

  return near < around ? near : around;
}

int v_space_compare(v_space_t v0, v_space_t v1)
{
  if(v0 < v1) {
    return -1;
  } else if(v0 > v1) {
    return 1;
  } else {
    return 0;
  }
}
