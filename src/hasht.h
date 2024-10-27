/** 
 * name: hasht.h
 * author: danyou
 * notes: 
 *   Insert `#define HASHT_IMPL` in only one unit before including this file.
 */

// Definitions:

#ifndef hasht_h
#define hasht_h

#include "dynarr.h"

struct hasht_entry {
    void* key;
    void* val;
};

struct hasht {
    struct dynarr* buf;
};

size_t hash(void*);

#endif // hasht_h

// Implementation:

#ifdef HASHT_IMPL
#undef HASHT_IMPL

#include <stdint.h>

int hasht_init(struct hasht* table)
{
    table->buf = malloc(sizeof(struct dynarr));
    if (!table->buf || !dynarr_init(table->buf))
        return 0;
    if (!dynarr_resize(table->buf, 128))
        return 0;
    return 1;
}

size_t _hash(void* key)
{
    return (size_t)((uintptr_t)key % 128);
}

#endif // HASHT_IMPL