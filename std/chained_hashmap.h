
#ifndef CHAINED_HASHMAP_H
#define CHAINED_HASHMAP_H

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct chained_hashmap chained_hashmap;
typedef struct chained_hashmap_vtable chained_hashmap_vtable;
typedef struct slot slot;

#define CHAINED_HASHMAP_INIT_SLOTS 64
#define MAX_CHAIN_LENGTH 4

struct slot {
    int key;
    int value;
    slot *next;
};

slot *slot_init(int key, int value) {
    slot *s = (slot *)malloc(sizeof(slot));
    s->key = key;
    s->value = value;
    s->next = nullptr;
    return s;
}

struct chained_hashmap {
    const chained_hashmap_vtable *vtable;
    int32_t count;
    int32_t capacity;
    slot **data;
};

static int32_t hash(chained_hashmap *hm, int key) {
    (void)hm; // just a dummy use
    // currently just using int value
    return (int32_t)key;
}

static int chained_hashmap_get(chained_hashmap *hm, int key) {
    int32_t idx = hash(hm, key) % hm->capacity;
    slot *s = hm->data[idx];

    while (true) {
        if (s == nullptr) { // not found
            abort();
        } else if (s->key == key) { // found
            return s->value;
        } else { // get next
            s = s->next;
        }
    }
}

static bool chained_hashmap_set(chained_hashmap *hm, int key, int value) {
    int32_t idx = hash(hm, key) % hm->capacity;

    slot *s = hm->data[idx];
    slot *p = nullptr;

    while (true) {
        if (s == nullptr) {
            s = slot_init(key, value);
            if (p != nullptr) {
                p->next = s;
            }
            return false;
        } else if (s->key == key) {
            s->value = value;
            return true;
        } else {
            p = s;
            s = s->next;
        }
    }
}

static void chained_hashmap_drop(chained_hashmap *hm) {
    for (int32_t idx = 0; idx < hm->capacity; idx++) {
        slot *s = hm->data[idx];
        while (s->next != nullptr) {
            slot *next = s->next;
            free(s);
            s = next;
        }
        free(s);
    }
}

struct chained_hashmap_vtable {
    int (*get)(chained_hashmap *self, int32_t key);
    bool (*set)(chained_hashmap *self, int32_t key, int32_t value);
    void (*drop)(chained_hashmap *self);
};

const chained_hashmap_vtable vtable = {
    .get = chained_hashmap_get,
    .set = chained_hashmap_set,
    .drop = chained_hashmap_drop,
};

static chained_hashmap chained_hashmap_rehash(chained_hashmap *hm) {
    int32_t capacity = hm->capacity * 2;
    slot **data = (slot **)calloc(capacity, sizeof(slot));
    for (int32_t idx = 0; idx < CHAINED_HASHMAP_INIT_SLOTS; idx++) {
        data[idx] = nullptr;
    }

    // here -->

    // move data
    // drop old
}

chained_hashmap chained_hashmap_init() {
    slot **data = (slot **)calloc(CHAINED_HASHMAP_INIT_SLOTS, sizeof(slot));
    for (int32_t idx = 0; idx < CHAINED_HASHMAP_INIT_SLOTS; idx++) {
        data[idx] = nullptr;
    }
    return (chained_hashmap){.vtable = &vtable, .count = 0, .capacity = CHAINED_HASHMAP_INIT_SLOTS, .data = data};
}

#endif // CHAINED_HASHMAP_H
