#ifndef TESTING_CSIM_H
#define TESTING_CSIM_H

#include "custom_structs.h"

void test_init_cache(void);
void test_does_set_contain(void);
void test_does_cache_contain(void);
void test_extract_set_bits(void);
void cache_print(cache* a_cache);

#endif