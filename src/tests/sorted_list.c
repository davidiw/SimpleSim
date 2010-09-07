/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Tests sorted_list
 */

#include <stdlib.h>
#include "CuTest.h"
#include "../sorted_list.h"
#include "../safe.h"

/**
 * Compares the values pointed to by int pointers.
 */
int compare_objects(int *v0, int *v1)
{
  if(*v0 < *v1) {
    return -1;
  } else if(*v0 > *v1) {
    return 1;
  } else {
    return 0;
  }
}

/**
 * Adds values in decreasing order.
 */
void sorted_list_decreasing_add_test(CuTest *tc)
{
  struct sorted_list *list = sorted_list_init(50, sizeof(int *),
      (int (*)(void *, void *)) compare_objects,
      (int (*)(void *, void *)) compare_objects,
      free);

  int vals[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int idx = 9;

  for(; idx >= 0; idx--) {
    sorted_list_add_element(list, &(vals[idx]));
  }

  for(idx = 0; idx < 10; idx++) {
    CuAssertIntEquals(tc, idx, * (int *) list->entries[idx]);
  }

  sorted_list_free(list, 0);
}

/**
 * Adds values in increasing order.
 */
void sorted_list_increasing_add_test(CuTest *tc)
{
  struct sorted_list *list = sorted_list_init(50, sizeof(int *),
      (int (*)(void *, void *)) compare_objects,
      (int (*)(void *, void *)) compare_objects,
      free);

  int vals[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int idx = 0;

  for(; idx < 10; idx++) {
    sorted_list_add_element(list, &(vals[idx]));
  }

  for(idx = 0; idx < 10; idx++) {
    CuAssertIntEquals(tc, idx, * (int *) list->entries[idx]);
    CuAssertIntEquals(tc, idx, sorted_list_get_index(list, &idx));
  }

  sorted_list_free(list, 0);
}

/**
 * Adds values in an unordered fashion.
 */
void sorted_list_random_add_test(CuTest *tc)
{
  struct sorted_list *list = sorted_list_init(50, sizeof(int *),
      (int (*)(void *, void *)) compare_objects,
      (int (*)(void *, void *)) compare_objects,
      (void (*)(void *)) free);

  int vals[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  sorted_list_add_element(list, &(vals[1]));
  sorted_list_add_element(list, &(vals[4]));
  sorted_list_add_element(list, &(vals[7]));
  sorted_list_add_element(list, &(vals[0]));
  sorted_list_add_element(list, &(vals[2]));
  sorted_list_add_element(list, &(vals[5]));
  sorted_list_add_element(list, &(vals[8]));
  sorted_list_add_element(list, &(vals[3]));
  sorted_list_add_element(list, &(vals[6]));
  sorted_list_add_element(list, &(vals[9]));

  int idx = 0;
  for(idx = 0; idx < 10; idx++) {
    CuAssertIntEquals(tc, idx, * (int *) list->entries[idx]);
  }

  sorted_list_free(list, 0);
}

/**
 * Tests for random insertion, removal, searching, and bounds.
 */
void sorted_list_random_test(CuTest *tc)
{
  struct sorted_list *list = sorted_list_init(50, sizeof(int *),
      (int (*)(void *, void *)) compare_objects,
      (int (*)(void *, void *)) compare_objects,
      (void (*)(void *)) free);
  int idx = 0;
  srand(time(0));
  for(; idx < list->capacity; idx++) {
    int *val = Malloc(sizeof(int));
    *val = rand();
    sorted_list_add_element(list, val);
  }

  int removed[10];
  for(idx = 0; idx < 10; idx++) {
    int to_remove = rand() % list->size;
    removed[idx] = to_remove;
    sorted_list_remove_index(list, to_remove);
  }

  for(idx = 0; idx < 10; idx++) {
    CuAssertTrue(tc, sorted_list_get_index(list, &(removed[idx])) < 0);
  }

  int prev = *(int *) list->entries[0];
  for(idx = 0; idx < list->size; idx++) {
    int current = *(int *) list->entries[idx];
    CuAssertTrue(tc, prev <= current);
    CuAssertIntEquals(tc, idx, sorted_list_get_index(list, &current));
    prev = current;
  }
}

CuSuite *cu_sorted_list()
{
	CuSuite *suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, sorted_list_increasing_add_test);
	SUITE_ADD_TEST(suite, sorted_list_decreasing_add_test);
	SUITE_ADD_TEST(suite, sorted_list_random_add_test);
	SUITE_ADD_TEST(suite, sorted_list_random_test);

	return suite;
}
