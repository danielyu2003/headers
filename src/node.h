/** 
 * name: node.h
 * author: danyou
 * notes: 
 *   Insert `#define NODE_IMPL` in only one unit before including this file.
 */

// Definitions:

#ifndef node_h
#define node_h

#include "dynarr.h"

struct node {
    void*           v;
    struct dynarr   c;
};

int node_init(struct node*, void* v);

#endif // node_h

// Implementation:

#ifdef NODE_IMPL
#undef NODE_IMPL

// ...

#endif // NODE_IMPL