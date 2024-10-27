/** 
 * name: hasht.h
 * author: danyou
 * notes: 
 *   Insert `#define HASHT_IMPL` in only one unit before including this file.
 */

// Definitions:

#ifndef hasht_h
#define hasht_h

struct hasht_entry {
    void* key;
    void* val;
};

struct hasht {
    struct dynarr*  buf;
    size_t          size;
    size_t          (*hash_fn)(void* key);                  // user defined!
    void            (*free_hasht_entry)(void* entry);       // user defined!
    int             (*hasht_entry_eq)(void* l, void* r);    // user defined!
};

int hasht_init(struct hasht*, size_t*);
int hasht_free(struct hasht*);
struct hasht_entry* hasht_get(struct hasht*, void*);
int hasht_set(struct hasht*, struct hasht_entry);

#endif // hasht_h

// Implementation:

#ifdef HASHT_IMPL
#undef HASHT_IMPL

#include <stdint.h>

#define DYNARR_IMPL
#include "dynarr.h"

void nested_free(void* dynarr)
{
    dynarr_free((struct dynarr *)dynarr);
    free(dynarr);
}

// void free_local(void* entry) 
// { 
//     // Additional cleanup here...
//     free(entry);
//     return; 
// }

// Important: Set user defined functions before calling!
int hasht_init(struct hasht* table, size_t* size)
{
    if (table->free_hasht_entry == NULL || table->hasht_entry_eq == NULL)
        return 0;
    size_t hasht_size = 128;
    if (size != NULL)
        hasht_size = *size;
    table->buf = malloc(sizeof(struct dynarr));
    struct dynarr* hasht_buf = table->buf;
    if (hasht_buf == NULL)
        return 0;
    
    hasht_buf->free_entry = nested_free;
    hasht_buf->entry_eq = table->hasht_entry_eq;
    
    if (!dynarr_init(hasht_buf, NULL)) {
        free(hasht_buf);
        hasht_buf = NULL;
        return 0;
    }
    if (!dynarr_resize(hasht_buf, hasht_size)) {
        free(hasht_buf->data);
        free(hasht_buf);
        hasht_buf = NULL;
        return 0;
    }
    table->size = hasht_size;
    return 1;
}

int hasht_free(struct hasht* table)
{
    dynarr_free(table->buf);
    free(table->buf);
    table->buf = NULL;
    table->size = 0;
    return 1;
}

struct hasht_entry* hasht_get(struct hasht* table, void* key)
{
    size_t hashval = table->hash_fn(key) % table->size;
    struct dynarr* entry_arr = (struct dynarr*)table->buf->data[hashval];
    if (entry_arr == NULL)
        return NULL;
    size_t entry_ind;
    if (dynarr_search(entry_arr, key, &entry_ind))
        return (struct hasht_entry*)entry_arr->data[entry_ind];
    return NULL;
}

int hasht_set(struct hasht* table, struct hasht_entry item)
{
    size_t hashval = table->hash_fn(item.key) % table->size;
    struct dynarr* entry_arr = (struct dynarr*)table->buf->data[hashval];
    if (entry_arr == NULL) {
        entry_arr = malloc(sizeof(struct dynarr));
        if (entry_arr == NULL)
            return 0;
        entry_arr->free_entry = table->free_hasht_entry;
        entry_arr->entry_eq = table->hasht_entry_eq;
        if (!dynarr_init(entry_arr, NULL)) {
            free(entry_arr);
            return 0;
        }
        table->buf->data[hashval] = entry_arr;
    }
    size_t entry_ind;
    if (dynarr_search(entry_arr, item.key, &entry_ind)) {
        ((struct hasht_entry*)entry_arr->data[entry_ind])->val = item.val;
        return 1;
    }
    struct hasht_entry* new_entry = malloc(sizeof(struct hasht_entry));
    if (new_entry == NULL)
        return 0;
    new_entry->key = item.key;
    new_entry->val = item.val;
    return dynarr_push(entry_arr, (void*)new_entry);
}

#endif // HASHT_IMPL