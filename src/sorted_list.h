/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * O(1) reads when using index, O(log N) when searching for an entry,
 * O(N) writes.  It is quite similar to the .NET SortedList.
 */

#ifndef _SORTED_LIST_H
#define _SORTED_LIST_H
#include <sys/types.h>

struct sorted_list {
  int (*compare_objects)(void *v0, void *v1);
  int (*compare_by_identifier)(void *v0, void *v1);
  void (*free_object)(void *v);
  void **entries;
  int size;
  int capacity;
  size_t entry_size;
};

struct sorted_list *sorted_list_init(int capacity, size_t entry_size,
    int (*compare_objects)(void *v0, void *v1), 
    int (*compare_by_identifier)(void *v0, void *v1),
    void (*free_object)(void *v));
void sorted_list_free(struct sorted_list *list, int entries);
int sorted_list_add_element(struct sorted_list *list, void *entry);
int sorted_list_get_index(struct sorted_list *list, void *entry);
int sorted_list_remove_element(struct sorted_list *list, void *entry);
int sorted_list_remove_index(struct sorted_list *list, int idx);
#endif
