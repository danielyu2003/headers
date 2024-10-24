#include <stdio.h>
#include <stdlib.h>

#define DYNARR_IMPL
#include "dynarr.h"

int main(void)
{
    struct dynarr x;

    dynarr_init(&x);

    return 0;
}