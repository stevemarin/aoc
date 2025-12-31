
#ifndef FNV1A_H
#define FNV1A_H

#include <cstdint>
#include <stddef.h>
#include <stdint.h>

static const uint64_t fnv1a_prime = 0x00000100000001b3;
static const uint64_t fnv1a_offset = 0xcbf29ce484222325;

uint64_t fnv1a(const uint8_t *data, uint64_t len) {
    uint64_t hash = fnv1a_offset;
    for (uint64_t idx = 0; idx < len; idx++) {
        hash ^= data[idx];
        hash *= fnv1a_prime;
    }
    return hash;
}

#endif // FNV1A_H
