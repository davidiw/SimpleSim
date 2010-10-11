/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Implements event_queue.h
 */

#include <glib.h>
#include "event_queue.h"

event_queue *event_queue_init()
{
  event_queue *equeue = (event_queue *) g_malloc(sizeof(event_queue));
  equeue->queue = g_sequence_new((void (*)(gpointer)) event_queue_entry_free);
  return equeue;
}

void event_queue_free(event_queue *equeue)
{
  g_sequence_free(equeue->queue);
  g_free(equeue);
}

void event_queue_enqueue(event_queue *queue, guint time, void (*func)(guint, void *), void *data)
{
  event_queue_entry *entry = g_slice_alloc0(sizeof(event_queue_entry));
  entry->time = time;
  entry->func = func;
  entry->data = data;
  g_sequence_insert_sorted(queue->queue, entry, (gint (*)(gconstpointer,
          gconstpointer, gpointer)) event_queue_entry_compare, NULL);
}

guint event_queue_execute(event_queue *equeue, guint time)
{
  GSequence *queue = equeue->queue;
  GSequenceIter *first_iter = g_sequence_get_begin_iter(queue);
  GSequenceIter *current_iter = first_iter;
  event_queue_entry *entry = NULL;

  while(g_sequence_iter_is_end(current_iter) == FALSE) {
    entry = (event_queue_entry *) g_sequence_get(current_iter);
    if(entry->time > time) {
      break;
    }
    current_iter = g_sequence_iter_next(current_iter);
    if(entry->func != NULL) {
      entry->func(time, entry->data);
    }
  }

  g_sequence_remove_range(first_iter, current_iter);
  if(g_sequence_get_length(queue) == 0) {
    return G_MAXUINT;
  }

  return entry->time;
}

gint event_queue_entry_compare(event_queue_entry *e0, event_queue_entry *e1)
{
  guint v0 = e0->time, v1 = e1->time;
  if(v0 < v1) {
    return -1;
  } else if(v0 > v1) {
    return 1;
  } else {
    return 0;
  }
}

void event_queue_entry_free(event_queue_entry *entry)
{
  g_slice_free1(sizeof(event_queue_entry), entry);
}
