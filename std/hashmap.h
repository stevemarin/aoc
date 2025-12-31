
#ifndef HASHMAP_H
#define HASHMAP_H

#include "owning_array.h"
#include <stdint.h>

typedef struct hashmap hashmap;
typedef struct hashmap_vtable hashmap_vtable;

#define HASHMAP_INIT_ARRAYS 32

struct hashmap {
    const hashmap_vtable *vtable;
    int32_t num_buckets;
    owning_array *buckets;
};

static int32_t hash(hashmap *hm, int key) {
    // currently just using int value
    return (int32_t)key % hm->num_buckets;
}

static int get(hashmap *hm, int key) {
    int32_t bucket_idx = 
}

struct hashmap_vtable {

};

hashmap hashmap_init() {
    
}

#endif // HASHMAP_H
