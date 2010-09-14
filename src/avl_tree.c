#include <stdlib.h>
#include "avl_tree.h"

int avl_tree_find(struct avl_tree *list, void *entry,
    struct avl_tree_entry *nearest_left, struct avl_tree_entry *nearest_right,
    struct avl_tree_entry *nearest)
{
  struct avl_tree_entry *cent = list->root;
  nearest = cent;

  while(cent != NULL) {
    nearest = cent;
    int cmp = list->compare(entry, cent->entry);
    if(cmp < 0) {
      nearest_left = cent;
      cent = cent->left;
    } else if(cmp > 0) {
      nearest_right = cent;
      cent = cent->right;
    } else {
      return 0;
    }
  }

  return -1;
}

void avl_tree_merge(struct avl_tree *list, struct avl_tree_entry *entries)
{
  if(entries->left != NULL) {
    avl_tree_merge(list, entries->left);
  }

  if(entries->right != NULL) {
    avl_tree_merge(list, entries->right);
  }

  avl_tree_insert(list, entries->entry);
}

void avl_tree_entry_free(struct avl_tree *list, struct avl_tree_entry *entries)
{
  if(entries->left != NULL) {
    avl_tree_entry_free(list, entries->left);
  }

  if(entries->right != NULL) {
    avl_tree_entry_free(list, entries->right);
  }

  if(list->free_object != NULL) {
    list->free_object(entries->entry);
  }

  free(entries);
}

struct avl_tree *avl_tree_init(size_t entry_size,
    int (*compare)(void *e0, void *e1), 
    int (*compare_by_identifier)(void *entry, void *id),
    void (*free_object)(void *entry))
{
  struct avl_tree *list = Malloc(sizeof(struct avl_tree));
  list->compare = compare;
  list->compare_by_identifier = compare_by_identifier;
  list->free_object = free_object;
  list->root = NULL;
  list->size = 0;
  list->entry_size = entry_size;
  return list;
}

void avl_tree_free(struct avl_tree *list)
{
  if(list->root != NULL) {
    avl_tree_entry_free(list, list->root);
  }
  free(list);
}

void *avl_tree_search(struct avl_tree *list, void *entry)
{
  struct avl_tree_entry *nearest_left, *nearest_right, *nearest;
  if(avl_tree_find(list, entry, nearest_left, nearest_right, nearest) != 0) {
    return NULL;
  }
  return nearest->entry;
}

int avl_tree_insert(struct avl_tree *list, void *entry)
{
  struct avl_tree_entry *nearest_left, *nearest_right, *nearest;
  if(avl_tree_find(list, entry, nearest_left, nearest_right, nearest) == 0) {
    perror("Entry already exists...");
    return -1;
  }

  struct avl_tree_entry *new_ent = Malloc(sizeof(struct avl_tree_entry));
  new_ent->entry = entry;
  if(list->compare(entry, nearest->entry) < 0) {
    nearest->left = new_ent;
  } else {
    nearest->right = new_ent;
  }

  return 0;
}

int avl_tree_delete(struct avl_tree *list, void *entry)
{
  struct avl_tree_entry *nearest_left, *nearest_right, *nearest;
  if(avl_tree_find(list, entry, nearest_left, nearest_right, nearest) != 0) {
    perror("No entry.");
    return -1;
  }

  if(nearest_left->left == nearest) {
    nearest_left->left = NULL;
  } else if(nearest_left->right == nearest) {
    nearest_left->right = NULL;
  } else if(nearest_right->left == nearest) {
    nearest_right->left = NULL;
  } else {
    nearest_right->right = NULL;
  }

  avl_tree_merge(list, nearest);
}
