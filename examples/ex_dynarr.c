#include <stdio.h>
#include <stdlib.h>

#define DYNARR_IMPL
#include "dynarr.h"

int int_eq(void* l, void* r) { return *(int*)l == *(int*)r; }
void free_local(void* entry) { return; }

int main(void)
{
    int a = 2;
    int b = 4;
    int c = 6;

    struct dynarr x;
    x.free_entry = free_local;
    x.entry_eq = int_eq;
    dynarr_init(&x, NULL);

    for (size_t i = 0; i < 2; i++) {
        dynarr_push(&x, &a);
        dynarr_push(&x, &b);
        dynarr_push(&x, &c);
    }
    printf("{%d %d %d}\n", *(int*)x.data[0], *(int*)x.data[1], *(int*)x.data[2]);

    size_t ind;
    dynarr_search(&x, &b, &ind);
    
    dynarr_delete(&x, ind);

    printf("{%d %d}\n", *(int*)x.data[0], *(int*)x.data[1]);

    dynarr_free(&x);

    return EXIT_SUCCESS;
}