/** 
 * name: dynarr.h
 * author: danyou
 * notes: 
 *   Insert `#define DYNARR_IMPL` in only one unit before including this file.
 */

// Definitions:

#ifndef dynarr_h
#define dynarr_h

struct dynarr {
    size_t  used;
    size_t  size;
    void**  data;
};

int dynarr_init(struct dynarr*);
int dynarr_free(struct dynarr*);
int dynarr_resize(struct dynarr*, size_t);

#endif // dynarr_h

// Implementation:

#ifdef DYNARR_IMPL
#undef DYNARR_IMPL

#include <stdlib.h>

int dynarr_init(struct dynarr* arr)
{
    arr->used = 0;
    arr->size = 16;
    arr->data = malloc(arr->size * sizeof(void*));
    if (arr->data == NULL)
        return 1;
    return 0;
}

int dynarr_free(struct dynarr* arr)
{
    arr->size = 0;
    arr->used = 0;
    free(arr->data);
    arr->data = NULL;
    return 0;
}

int dynarr_resize(struct dynarr* arr, size_t n)
{
    void** tmp = realloc(arr->data, n * sizeof(void*));
    if (tmp == NULL)
        return 1;
    if (n < arr->used)
        arr->used = n;
    arr->data = tmp;
    arr->size = n;
    return 0;
}

#endif // DYNARR_IMPL
