#include <stdint.h>
#include <stdio.h>

#include "chained_hashmap.h"
#include "owning_array.h"

#define __DEFER__(F, V)                                                                                               \
    auto void F(int *);                                                                                               \
    [[gnu::cleanup(F)]] int V;                                                                                        \
    auto void F(int *)

#define defer __DEFER(__COUNTER__)
#define __DEFER(N) __DEFER_(N)
#define __DEFER_(N) __DEFER__(__DEFER_FUNCTION_##N, __DEFER_VARIABLE_##N)

int main() {
    owning_array arr = owning_array_init();
    for (int idx = 0; idx < 9; idx++) {
        arr.vtable->append(&arr, idx);
    }

    for (int idx = 0; idx < arr.count; idx++) {
        printf("%i -> %i\n", idx, arr.data[idx]);
    }

    arr.vtable->drop(&arr);

    chained_hashmap hm = chained_hashmap_init();
    hm.vtable->set(&hm, 1, 1);
    hm.vtable->set(&hm, 2, 2);
    hm.vtable->set(&hm, 3, 3);
    hm.vtable->set(&hm, 4, 4);
    hm.vtable->set(&hm, 5, 5);
    hm.vtable->set(&hm, 1, 1);
    hm.vtable->set(&hm, 2, 2);
    hm.vtable->set(&hm, 3, 3);
    hm.vtable->set(&hm, 4, 4);
    hm.vtable->set(&hm, 5, 5);
    int32_t val = hm.vtable->get(&hm, 5);
    printf("AAA: %i\n", val);

    hm.vtable->drop(&hm);

    return 0;
}