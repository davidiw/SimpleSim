/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * An event queue that will execute a callback at the specified time
 */

#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

typedef struct {
  guint time;
  void (*func)(guint, void *);
  void *data;
} event_queue_entry;

typedef struct {
  GSequence *queue;
} event_queue;

event_queue *event_queue_init();
void event_queue_free(event_queue *equeue);
void event_queue_enqueue(event_queue *queue, guint time, void (*func)(guint, void *), void *data);
guint event_queue_execute(event_queue *equeue, guint time);

gint event_queue_entry_compare(event_queue_entry *e0, event_queue_entry *e1);
void event_queue_entry_free(event_queue_entry *entry);
#endif
