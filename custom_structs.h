#ifndef CUSTOM_STRUCTS_H
#define CUSTOM_STRUCTS_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool valid_bit;
    uint64_t tag;
    uint64_t last_used;
} line;
typedef struct {
    line* lines;
    uint64_t num_lines;
} set;
typedef struct {
    set* sets;
    uint8_t tag_bits_num;
    uint8_t set_bits_num;
    uint64_t e;
    uint64_t num_sets;
} cache;

#endif