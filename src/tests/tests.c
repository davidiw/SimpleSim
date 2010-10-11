#include <glib.h>

void test_graph();

int main(int argc, char **argv)
{ 
  g_test_init(&argc, &argv, NULL);
  test_graph();
  test_event_queue();
  return g_test_run();
}
