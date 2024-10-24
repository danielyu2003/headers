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
    size_t  used;   // count of elements present
    size_t  size;   // total number of elements
    void**  data;   // array buffer
};

int dynarr_init(struct dynarr*);
int dynarr_free(struct dynarr*);
int dynarr_resize(struct dynarr*, size_t);
int dynarr_append(struct dynarr*, void*);


#endif // dynarr_h

// Implementation:

#ifdef DYNARR_IMPL
#undef DYNARR_IMPL

#include <stdlib.h>

int dynarr_init(struct dynarr* arr)
{
    arr->data = calloc(16, sizeof(void*));
    if (arr->data == NULL)
        return 1;
    arr->size = 16;
    arr->used = 0;
    return 0;
}

int dynarr_free(struct dynarr* arr)
{
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->used = 0;
    return 0;
}

int dynarr_resize(struct dynarr* arr, size_t n)
{
    void** tmp = realloc(arr->data, n * sizeof(void*));
    if (tmp == NULL)
        return 1;
    arr->data = tmp;
    arr->size = n;
    if (n < arr->used) arr->used = n;
    return 0;
}

int dynarr_append(struct dynarr* arr, void* item)
{
    if (arr->used == arr->size) {
        if (dynarr_resize(arr, arr->size*2) == 1)
            return 1;
    }
    arr->data[arr->used++] = item;
    return 0;
}

#endif // DYNARR_IMPL
