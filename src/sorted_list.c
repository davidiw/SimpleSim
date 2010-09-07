/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * Implements sorted_list.h
 */

#include <string.h>
#include "sorted_list.h"
#include "safe.h"

/* Helper functions */

struct sorted_list *expand(struct sorted_list *list)
{
  perror("Not implemented");
  exit(-1);
  return list;
}

/* Public functions */

struct sorted_list *sorted_list_init(int capacity, size_t entry_size,
    int (*compare_objects)(void *v0, void *v1), 
    int (*compare_by_identifier)(void *v0, void *v1),
    void (*free_object)(void *v))
{
  struct sorted_list *list = Malloc(sizeof(struct sorted_list));
  list->size = 0;
  list->capacity = capacity;
  list->entry_size = entry_size;
  list->entries = Malloc(entry_size * capacity);
  list->compare_objects = compare_objects;
  list->compare_by_identifier = compare_by_identifier;
  list->free_object = free_object;
  return list;
}

void sorted_list_free(struct sorted_list *list, int entries)
{
  if(list == NULL) {
    perror("NULL list!");
    return;
  }

  if(list->entries == NULL) {
    perror("Entries already emptied!");
  } else {
    if(entries > 0) {
      int idx = 0;
      for(; idx < list->size; idx++) {
        list->free_object(list->entries[idx]);
      }
    }
    free(list->entries);
  }

  free(list);
}

int sorted_list_add_element(struct sorted_list *list, void *entry)
{
  int cidx = sorted_list_get_index(list, entry);
  if(cidx < 0) {
    cidx = ~cidx;
  }

  /* Increment the size and determine if the capacity is still large enough. */
  if(list->capacity == list->size) {
    list = expand(list);
  }

  if(cidx < list->size) {
    memmove(&(list->entries[cidx + 1]), &(list->entries[cidx]), (list->size - cidx) * list->entry_size);
  }

  list->entries[cidx] = entry;
  list->size++;
  return cidx;
}

int sorted_list_get_index(struct sorted_list *list, void *entry)
{
  int lower = 0, upper = list->size - 1, pos = (upper - lower) / 2, cidx;

  while(upper >= lower && upper < list->size) {
    int cmp = list->compare_objects((void *) list->entries[pos], entry);
    if(cmp < 0) {
      lower = pos + 1;
    } else if(cmp > 0) {
      upper = pos - 1;
    } else {
      break;
    }
    pos = (upper - lower) / 2;
    pos += lower;
  }

  if(upper < lower) {
    if(pos != list->size) {
      if(list->compare_objects((void *) list->entries[pos], entry) < 0) {
        pos++;
      }
    }
    pos = (pos + 1) * -1;
  }

  return pos;
}

int sorted_list_remove_element(struct sorted_list *list, void *entry)
{
  int idx = sorted_list_get_index(list, entry);
  if(idx < 0) {
    return -1;
  }
  return sorted_list_remove_index(list, idx);
}

int sorted_list_remove_index(struct sorted_list *list, int idx)
{
  if(idx >= list->size) {
    return -1;
  }

  memmove(&(list->entries[idx]), &(list->entries[idx + 1]),
      (list->size - idx - 1) * list->entry_size);
  list->size--;
  return 0;
}
