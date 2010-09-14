/**
 * @file
 * @author David Wolinsky <isaac.wolinsky@gmail.com>
 * @section LICENSE
 * See LICENSE in root directory
 * @section DESCRIPTION
 * AVL tree implementation -- a self-balancing binary search tree
 * Search - O(log N), Insert - O(log N), Delete - O(log N)
 */

#ifndef _SORTED_LIST_H
#define _SORTED_LIST_H
#include <sys/types.h>

struct avl_tree_entry {
  void *entry;
  struct avl_tree_entry *left;
  struct avl_tree_entry *right;
};

struct avl_tree {
  int (*compare)(void *e0, void *e1);
  int (*compare_by_identifier)(void *ent, void *id);
  void (*free_object)(void *ent);
  struct avl_tree_entry *root;
  int size;
  size_t entry_size;
};

struct avl_tree *avl_tree_init(size_t entry_size,
    int (*compare)(void *e0, void *e1), 
    int (*compare_by_identifier)(void *entry, void *id),
    void (*free_object)(void *entry));

void avl_tree_free(struct avl_tree *list);
int avl_tree_insert(struct avl_tree *list, void *entry);
void *avl_tree_search(struct avl_tree *list, void *entry);
int avl_tree_delete(struct avl_tree *list, void *entry);
#endif
