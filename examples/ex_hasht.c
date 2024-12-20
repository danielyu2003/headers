#include <stdio.h>
#include <stdlib.h>

#define HASHT_IMPL
#include "hasht.h"

int int_cmp(void* key, void* entry) 
{
    int lint = *(int*)key;
    int rint = *(int*)((struct hasht_entry*)entry)->key;
    if (lint > rint)
        return 1;
    if (lint < rint)
        return -1;
    return 0;
}

size_t identity(void* key)
{
    return *(size_t*)key;
}

void repr(struct hasht_entry* ent)
{
    printf("{key: %d, val: %d}\n", *(int*)ent->key, *(int*)ent->val);
}

void printhasht(struct hasht* t)
{
    for (size_t i = 0; i < t->size; i++) {
        struct dynarr* curr_bin = t->buf->data[i];
        if (curr_bin == NULL)
            continue;
        for (size_t j = 0; j < curr_bin->used; j++) {
            struct hasht_entry* curr_entry = curr_bin->data[j];
            if (curr_entry == NULL)
                continue;
            repr(curr_entry);
        }
    }
}

int main(void)
{
    int a = 2;
    int b = 4;
    int c = 6;

    struct hasht_entry aa = { .key = &a, .val = &a }; // 2, 2
    struct hasht_entry ab = { .key = &a, .val = &b }; // 2, 4
    struct hasht_entry ac = { .key = &a, .val = &c }; // 2, 6 <--
    struct hasht_entry ba = { .key = &b, .val = &a }; // 4, 2
    struct hasht_entry bb = { .key = &b, .val = &b }; // 4, 4 <--
    struct hasht_entry ca = { .key = &c, .val = &a }; // 6, 2 <--

    struct hasht x = {0};
    x.cmp_hasht_key = int_cmp;
    x.hash = identity;
    hasht_init(&x, NULL);

    hasht_set(&x, aa);
    hasht_set(&x, ab);
    hasht_set(&x, ac);
    hasht_set(&x, ba);
    hasht_set(&x, bb);
    hasht_set(&x, ca);

    printhasht(&x);

    printf("Get Key: %d = Val: %d\n", b, *(int*)hasht_get(&x, &b));

    hasht_del(&x, &b);

    printhasht(&x);

    hasht_free(&x);
    return EXIT_SUCCESS;
}