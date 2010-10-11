#include <glib.h>
#include "../event_queue.h"

void test_event_queue_init()
{
  event_queue *eq = event_queue_init();
  event_queue_execute(eq, G_MAXUINT);
  event_queue_free(eq);
}

typedef struct {
  guint current_time;
  guint total;
} event_queue_test_struct;

void queued(guint time, event_queue_test_struct *status)
{
  g_assert(time <= status->current_time);
  status->total++;
}

void test_event_queue_random()
{
  guint idx = 0;
  GRand* grand = g_rand_new_with_seed(g_test_rand_int());
  event_queue *eq = event_queue_init();


  event_queue_test_struct status = {0, 0};

  for(; idx < 10000; idx++) {
    guint time = g_test_rand_int_range(5000, 10000);
    event_queue_enqueue(eq, time, (void (*)(guint, void *))queued, &status);
  }

  idx = 0;
  guint next = 0;
  while(next != G_MAXUINT) {
    status.current_time = next;
    next = event_queue_execute(eq, next);
  }

  g_assert(status.total == 10000);
  event_queue_free(eq);
}

//void event_queue_enqueue(event_queue *queue, guint time, void (*func)(void *), void *data);
//guint event_queue_execute(event_queue *equeue, guint time);

void test_event_queue()
{
  g_test_add_func("/event_queue/init", test_event_queue_init);
  g_test_add_func("/event_queue/random", test_event_queue_random);
}
