/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Tests avl_tree
 */

#include <stdlib.h>
#include "CuTest.h"
#include "../avl_tree.h"
#include "../safe.h"

/**
 * Compares the values pointed to by int pointers.
 */
int compare(int *v0, int *v1)
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
void avl_tree_decreasing_add_test(CuTest *tc)
{
  struct avl_tree *list = avl_tree_init(sizeof(int *),
      (int (*)(void *, void *)) compare,
      (int (*)(void *, void *)) compare,
      free);

  int vals[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int idx = 9;

  for(; idx >= 0; idx--) {
    avl_tree_insert(list, &(vals[idx]));
  }

  for(idx = 0; idx < 10; idx++) {
//    CuAssertIntEquals(tc, idx, * (int *) list->entries[idx]);
  }

  avl_tree_free(list);
}

/**
 * Adds values in increasing order.
 */
void avl_tree_increasing_add_test(CuTest *tc)
{
  struct avl_tree *list = avl_tree_init(sizeof(int *),
      (int (*)(void *, void *)) compare,
      (int (*)(void *, void *)) compare,
      free);

  int vals[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int idx = 0;

  for(; idx < 10; idx++) {
    avl_tree_insert(list, &(vals[idx]));
  }

  for(idx = 0; idx < 10; idx++) {
//    CuAssertIntEquals(tc, idx, * (int *) list->entries[idx]);
//    CuAssertIntEquals(tc, idx, avl_tree_get_index(list, &idx));
  }

  avl_tree_free(list);
}

/**
 * Adds values in an unordered fashion.
 */
void avl_tree_random_add_test(CuTest *tc)
{
  struct avl_tree *list = avl_tree_init(sizeof(int *),
      (int (*)(void *, void *)) compare,
      (int (*)(void *, void *)) compare,
      (void (*)(void *)) free);

  int vals[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  avl_tree_insert(list, &(vals[1]));
  avl_tree_insert(list, &(vals[4]));
  avl_tree_insert(list, &(vals[7]));
  avl_tree_insert(list, &(vals[0]));
  avl_tree_insert(list, &(vals[2]));
  avl_tree_insert(list, &(vals[5]));
  avl_tree_insert(list, &(vals[8]));
  avl_tree_insert(list, &(vals[3]));
  avl_tree_insert(list, &(vals[6]));
  avl_tree_insert(list, &(vals[9]));

  int idx = 0;
  for(idx = 0; idx < 10; idx++) {
//    CuAssertIntEquals(tc, idx, * (int *) list->entries[idx]);
  }

  avl_tree_free(list);
}

/**
 * Tests for random insertion, removal, searching, and bounds.
 */
void avl_tree_random_test(CuTest *tc)
{
  struct avl_tree *list = avl_tree_init(sizeof(int *),
      (int (*)(void *, void *)) compare,
      (int (*)(void *, void *)) compare,
      (void (*)(void *)) free);
  int idx = 0;
  srand(time(0));
  for(; idx < 50; idx++) {
    int *val = Malloc(sizeof(int));
    *val = rand();
    avl_tree_insert(list, val);
  }

  int removed[10];
  for(idx = 0; idx < 10; idx++) {
    int to_remove = rand() % list->size;
    removed[idx] = to_remove;
//    avl_tree_remove_index(list, to_remove);
  }

  for(idx = 0; idx < 10; idx++) {
//    CuAssertTrue(tc, avl_tree_get_index(list, &(removed[idx])) < 0);
  }

//  int prev = *(int *) list->entries[0];
  for(idx = 0; idx < list->size; idx++) {
//    int current = *(int *) list->entries[idx];
//    CuAssertTrue(tc, prev <= current);
//    CuAssertIntEquals(tc, idx, avl_tree_get_index(list, &current));
//    prev = current;
  }
}

CuSuite *cu_avl_tree()
{
	CuSuite *suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, avl_tree_increasing_add_test);
	SUITE_ADD_TEST(suite, avl_tree_decreasing_add_test);
	SUITE_ADD_TEST(suite, avl_tree_random_add_test);
	SUITE_ADD_TEST(suite, avl_tree_random_test);

	return suite;
}
