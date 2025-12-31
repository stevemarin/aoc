
#ifndef OWNING_ARRAY_H
#define OWNING_ARRAY_H

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#define OWNING_ARRAY_INIT_SIZE 4

typedef struct owning_array owning_array;
typedef struct owning_array_vtable owning_array_vtable;

struct owning_array {
    const owning_array_vtable *vtable;
    int64_t count;
    int64_t capacity;
    int *data;
};

static int owning_array_get(owning_array *arr, int32_t idx) {
    assert(0 <= idx && idx < arr->count);
    return arr->data[idx];
}

static int owning_array_set(owning_array *arr, int32_t idx, int value) {
    assert(0 <= idx && idx < arr->count);
    int prev = arr->data[idx];
    arr->data[idx] = value;
    return prev;
}

static void owning_array_append(owning_array *arr, int value) {
    if (arr->count == arr->capacity) {
        arr->capacity *= 2;
        arr->data = (int *)realloc(arr->data, sizeof(int) * arr->capacity);
    }
    arr->data[arr->count] = value;
    arr->count++;
}

static void owning_array_drop(owning_array *arr) { free(arr->data); }

struct owning_array_vtable {
    int (*get)(owning_array *self, int32_t idx);
    int (*set)(owning_array *self, int32_t idx, int value);
    void (*append)(owning_array *self, int value);
    void (*drop)(owning_array *self);
};

const owning_array_vtable owning_array_vtable_ = {
    .get = owning_array_get, .set = owning_array_set, .append = owning_array_append, .drop = owning_array_drop};

owning_array owning_array_init() {
    int *data = (int *)calloc(OWNING_ARRAY_INIT_SIZE, sizeof(int));
    return (owning_array){
        .vtable = &owning_array_vtable_, .count = 0, .capacity = OWNING_ARRAY_INIT_SIZE, .data = data};
}

#endif // OWNING_ARRAY_H
