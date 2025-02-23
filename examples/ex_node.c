
#include <stdio.h>
#include "node.h"
#include "dynarr.h"

int main(void)
{

    int v1 = 2;
    struct dynarr children = {0};
    struct node root = {.v = &v1, .c = children};

    printf("%d\n", *(int*)root.v);

    return 0;
}