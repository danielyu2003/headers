#include <stdio.h>
#include <stdlib.h>

#define DYNARR_IMPL
#include "dynarr.h"

int main(void)
{
    struct dynarr x;

    int a = 2;
    int b = 4;
    int c = 6;

    dynarr_init(&x);

    for (size_t i = 0; i < 256; i++) {
        dynarr_append(&x, &a);
        dynarr_append(&x, &b);
        dynarr_append(&x, &c);
    }
    
    printf("%zu\n", x.used);
    printf("%zu\n", x.size);

    dynarr_free(&x);

    return EXIT_SUCCESS;
}