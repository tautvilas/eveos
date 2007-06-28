#ifndef _RING_H_
#define _RING_H_

#include "global.h"

// Internal data types.
// Do not access them directly
typedef struct ds_ring_node_s {
        pointer_t               mpData;
        struct ds_ring_node_s*  mpNext;
    } ds_ring_node_t;

typedef struct {
        size_t          mSize;
        ds_ring_node_t* mpNode;
    } ds_ring_t;


ds_ring_t*
ds_ring_create();

void
ds_ring_destroy(ds_ring_t* apRing);

bool_t
ds_ring_ins(ds_ring_t* apRing, const pointer_t apData);

void
ds_ring_del(ds_ring_t* apRing);

pointer_t
ds_ring_next(ds_ring_t* apRing);

pointer_t
ds_ring_curr(const ds_ring_t* apRing);

pointer_t
ds_ring_find(ds_ring_t* apRing, const pointer_t apData);

bool_t
ds_ring_rem(ds_ring_t* apRing, const pointer_t apData);

#endif // _RING_H_
