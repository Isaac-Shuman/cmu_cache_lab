#include "testing_csim.h"
#include "csim.h"

#define check_extract_set_bits(address, set_bits_num, tag_bits_num, expected) \
do { \
uint64_t set_bits = extract_set_bits(address, set_bits_num, tag_bits_num); \
if ( set_bits != expected ) \
    printf("Address: %lX . set_bits expected: %lX . set_bits actual: %lX\n", address, (unsigned long) expected, set_bits); \
} while (0)
#define check_extract_tag_bits(address, set_bits_num, tag_bits_num, expected) \
do { \
uint64_t tag_bits = extract_tag_bits(address, set_bits_num, tag_bits_num); \
if ( tag_bits != expected ) \
    printf("Address: %lX . tag_bits expected: %lX . tag_bits actual: %lX\n", address, (unsigned long) expected, tag_bits); \
} while (0)

void set_print(set* a_set);

void set_print(set* a_set) {
    for (int64_t i = 0; i < a_set->num_lines; i++)
        if(a_set->lines[i].valid_bit)
            printf("\t\tLine #%lu contains tag %#lX\n", i, a_set->lines[i].tag);

}
void cache_print(cache* a_cache) {
    puts("Cache");
    for (int64_t i = 0; i < a_cache->num_sets; i++) {
        printf("\tSet #%lu\n", i);
        set_print(&(a_cache->sets[i]));
    }
}

void test_extract_set_bits(void) {
    //uint64_t address = 0xFFFFFFFFFFFFFFFF;
    check_extract_set_bits(0xFFFFFFFFFFFFFFFF, 1, 1, 1);
    check_extract_tag_bits(0xFFFFFFFFFFFFFFFF, 1, 1, 1);
    //uint64_t set_bits = extract_set_bits(address, 1, 1);
    // if (set_bits != 1)
    //     printf("Address: %lX set_bits expected: %lX set_bits actual: %lX\n", address, (unsigned long) 1, set_bits);
}

void test_init_cache(void) {
    cache a_cache, *a_cache_p = &a_cache;
    init_cache(a_cache_p, 2, 2, 62);
    cache_print(a_cache_p);
}