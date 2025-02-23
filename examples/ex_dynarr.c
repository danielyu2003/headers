#include <stdio.h>
#include <stdlib.h>

#define DYNARR_IMPL
#include "dynarr.h"

int int_cmp(const void* l, const void* r) {
    int lint = *(int*)l;
    int rint = *(int*)r;
    if (lint > rint)
        return 1;
    if (lint < rint)
        return -1;
    return 0;
}

int main(void)
{
    int a = 6;
    int b = 2;
    int c = 1;
    int d = 4;

    struct dynarr x = {0};
    x.entry_cmp = int_cmp;
    
    dynarr_init(&x, 0);

    for (size_t i = 0; i < 1; i++) {
        dynarr_push(&x, &a);
        dynarr_push(&x, &b);
        dynarr_push(&x, &c);
        dynarr_push(&x, &d);
    }

    printf("{%d %d %d %d}\n", *(int*)x.data[0], *(int*)x.data[1], *(int*)x.data[2], *(int*)x.data[3]);

    dynarr_sort(&x);

    printf("{%d %d %d %d}\n", *(int*)x.data[0], *(int*)x.data[1], *(int*)x.data[2], *(int*)x.data[3]);

    printf("used: %zu, size: %zu\n", x.used, x.size);

    size_t ind = 0;
    dynarr_search(&x, &b, &ind);
    
    dynarr_delete(&x, ind);

    printf("{%d %d %d}\n", *(int*)x.data[0], *(int*)x.data[1], *(int*)x.data[2]);

    printf("used: %zu, size: %zu\n", x.used, x.size);

    dynarr_free(&x);

    return EXIT_SUCCESS;
}