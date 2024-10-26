#include <stdio.h>
#include <stdlib.h>

#define DYNARR_IMPL
#include "dynarr.h"

int int_eq(void* l, void* r)
{
    return *(int*)l == *(int*)r;
}

int main(void)
{
    int a = 2;
    int b = 4;
    int c = 6;

    struct dynarr x;
    dynarr_init(&x);

    for (size_t i = 0; i < 1; i++) {
        dynarr_push(&x, &a);
        dynarr_push(&x, &b);
        dynarr_push(&x, &c);
    }
    printf("{%d %d %d}\n", *(int*)x.data[0], *(int*)x.data[1], *(int*)x.data[2]);

    size_t ind;
    dynarr_search(&x, &b, &ind, &int_eq);
    
    dynarr_delete(&x, ind);

    printf("{%d %d}\n", *(int*)x.data[0], *(int*)x.data[1]);

    dynarr_free(&x);

    return EXIT_SUCCESS;
}