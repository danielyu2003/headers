/** 
 * name: hasht.h
 * author: danyou
 * notes: 
 *   Insert `#define HASHT_IMPL` in only one unit before including this file.
 */

// Definitions:

#ifndef hasht_h
#define hasht_h

#include <stdlib.h>

struct hasht_entry {
    void* key;
    void* val;
};

struct hasht {
    struct dynarr* buf;
    size_t size;
    size_t (*hash)(void* key); // user defined!
    void (*free_hasht_entry)(void* entry); // user defined!
    int (*cmp_hasht_key)(void* key, void* entry); // user defined!
};

int hasht_init(struct hasht*, size_t*);
int hasht_free(struct hasht*);
int hasht_set(struct hasht*, struct hasht_entry);
void* hasht_get(struct hasht*, void*);
int hasht_del(struct hasht*, void*);

#endif // hasht_h

// Implementation:

#ifdef HASHT_IMPL
#undef HASHT_IMPL

#include <stdint.h>

#define DYNARR_IMPL
#include "dynarr.h"

// Default free for entries containing POD.
void pod_free(void* entry) 
{ 
    free(entry);
    return; 
}

void nested_free(void* dynarr)
{
    dynarr_free((struct dynarr *)dynarr);
    free(dynarr);
}

int no_cmp(void* a, void* b)
{
    return 0;
}

int hasht_init(struct hasht* table, size_t* size)
{
    if (table->free_hasht_entry == NULL)
        table->free_hasht_entry = pod_free;
    if (table->cmp_hasht_key == NULL || table->hash == NULL)
        return 0;
    size_t hasht_size = 128;
    if (size != NULL)
        hasht_size = *size;
    table->buf = malloc(sizeof(struct dynarr));
    struct dynarr* hasht_buf = table->buf;
    if (hasht_buf == NULL)
        return 0;
    hasht_buf->entry_free = nested_free;
    hasht_buf->entry_cmp = no_cmp;
    if (!dynarr_init(hasht_buf, NULL)) {
        free(hasht_buf);
        hasht_buf = NULL;
        return 0;
    }
    if (!dynarr_resize(hasht_buf, hasht_size)) {
        hasht_free(table);
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

void* hasht_get(struct hasht* table, void* key)
{
    size_t hashval = table->hash(key) % table->size;
    struct dynarr* entry_arr = (struct dynarr*)table->buf->data[hashval];
    if (entry_arr == NULL)
        return NULL;
    size_t entry_ind;
    if (dynarr_search(entry_arr, key, &entry_ind))
        return ((struct hasht_entry*)entry_arr->data[entry_ind])->val;
    return NULL;
}

int hasht_set(struct hasht* table, struct hasht_entry item)
{
    size_t hashval = table->hash(item.key) % table->size;
    struct dynarr* entry_arr = (struct dynarr*)table->buf->data[hashval];
    if (entry_arr == NULL) {
        entry_arr = malloc(sizeof(struct dynarr));
        if (entry_arr == NULL)
            return 0;
        entry_arr->entry_free = table->free_hasht_entry;
        entry_arr->entry_cmp = table->cmp_hasht_key;
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
    if (!dynarr_push(entry_arr, (void*)new_entry)) {
        free(new_entry);
        return 0;
    }
    return 1;
}

int hasht_del(struct hasht* table, void* key)
{
    size_t hashval = table->hash(key) % table->size;
    struct dynarr* entry_arr = (struct dynarr*)table->buf->data[hashval];
    if (entry_arr == NULL)
        return 0;
    size_t entry_ind;
    if (!dynarr_search(entry_arr, key, &entry_ind))
        return 0;
    if (dynarr_delete(entry_arr, entry_ind)) {
        if (entry_arr->used == 0) {
            dynarr_free(entry_arr);
            free(entry_arr);
            table->buf->data[hashval] = NULL;
        }
        return 1;
    }
    return 0;
}

#endif // HASHT_IMPL