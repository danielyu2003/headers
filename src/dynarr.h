/**
 * name: dynarr.h
 * author: danyou
 * notes:
 *   Insert `#define DYNARR_IMPL` in only one unit before including this file.
 */

#ifndef dynarr_h
#define dynarr_h

#include <stdlib.h>

struct dynarr {
  size_t used;                                    // count of elements present
  size_t size;                                    // total allocated slots
  void **data;                                    // array buffer
  void (*entry_free)(void *entry);                // user defined (optional)
  int (*entry_cmp)(const void *l, const void *r); // user defined (optional)
};

int dynarr_init(struct dynarr *, size_t);
int dynarr_free(struct dynarr *);
int dynarr_resize(struct dynarr *, size_t);
int dynarr_insert(struct dynarr *, void *, size_t);
int dynarr_delete(struct dynarr *, size_t);
int dynarr_push(struct dynarr *, void *);
void *dynarr_pop(struct dynarr *);
int dynarr_search(struct dynarr *, void *, size_t *);
int dynarr_sort(struct dynarr *);

#endif // dynarr_h

// Implementation:

#ifdef DYNARR_IMPL
#undef DYNARR_IMPL

#include <limits.h>
#include <stdlib.h>
#include <string.h>

static void static_free(void *entry) {
  (void)(entry);
  return;
}

int dynarr_init(struct dynarr *arr, size_t size) {
  if (arr->entry_free == NULL)
    arr->entry_free = static_free;

  size_t arr_size = (size == 0) ? 16 : size;

  arr->data = calloc(arr_size, sizeof(void *));
  if (arr->data == NULL)
    return 0;
  arr->size = arr_size;
  arr->used = 0;
  return 1;
}

int dynarr_resize(struct dynarr *arr, size_t n) {
  if (n > SIZE_MAX / sizeof(void *))
    return 0;

  // If shrinking, free truncated entries
  if (n < arr->used) {
    for (size_t i = n; i < arr->used; i++) {
      if (arr->data[i] != NULL)
        arr->entry_free(arr->data[i]);
    }
    arr->used = n;
  }

  void **tmp = realloc(arr->data, n * sizeof(void *));
  if (tmp == NULL && n > 0)
    return 0;

  // Zero out any new space
  if (n > arr->size)
    memset(&tmp[arr->size], 0, (n - arr->size) * sizeof(void *));

  arr->data = tmp;
  arr->size = n;
  return 1;
}

int dynarr_free(struct dynarr *arr) {
  // resizing to 0 frees memory + entries
  if (!dynarr_resize(arr, 0))
    return 0;

  arr->data = NULL; // safety
  arr->size = 0;
  arr->used = 0;
  return 1;
}

int dynarr_insert(struct dynarr *arr, void *item, size_t index) {
  if (index > arr->used)
    return 0;
  if (arr->used == arr->size) {
    if (!dynarr_resize(arr, arr->size * 2))
      return 0;
  }
  size_t n_move = arr->used - index;
  memmove(&arr->data[index + 1], &arr->data[index], n_move * sizeof(void *));
  arr->data[index] = item;
  arr->used++;
  return 1;
}

int dynarr_delete(struct dynarr *arr, size_t index) {
  if (index >= arr->used)
    return 0;
  arr->entry_free(arr->data[index]);
  size_t n_move = (arr->used - 1) - index;
  if (n_move > 0)
    memmove(&arr->data[index], &arr->data[index + 1], n_move * sizeof(void *));
  arr->data[--arr->used] = NULL;
  return 1;
}

int dynarr_push(struct dynarr *arr, void *item) {
  return dynarr_insert(arr, item, arr->used);
}

void *dynarr_pop(struct dynarr *arr) {
  if (arr->used == 0)
    return NULL;
  void *item = arr->data[arr->used - 1];
  arr->data[--arr->used] = NULL;
  return item;
}

int dynarr_search(struct dynarr *arr, void *item, size_t *index) {
  if (arr->entry_cmp == NULL || index == NULL)
    return 0;
  for (size_t i = 0; i < arr->used; i++) {
    if (arr->data[i] == NULL)
      continue;
    if (arr->entry_cmp(item, arr->data[i]) == 0) {
      *index = i;
      return 1;
    }
  }
  return 0;
}

int dynarr_sort(struct dynarr *arr) {
  if (arr->entry_cmp == NULL)
    return 0;
  for (size_t i = 1; i < arr->used; i++) {
    void *key = arr->data[i];
    ssize_t j = (ssize_t)i - 1;
    while (j >= 0 && arr->entry_cmp(arr->data[j], key) > 0) {
      arr->data[j + 1] = arr->data[j];
      j--;
    }
    arr->data[j + 1] = key;
  }
  return 1;
}

#endif // DYNARR_IMPL
