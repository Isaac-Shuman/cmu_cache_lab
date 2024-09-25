#ifndef CSIM_H
#define CSIM_H

#include "cachelab.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h> //for malloc
#include <unistd.h> //for getopt
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include "custom_structs.h"

#define WORD_SIZE 64
#define TRAIL(x) (((x) > 0) ? (UINT64_MAX >> (WORD_SIZE-(x))) : 0)
#define LEAD(x) (((x) > 0) ? (UINT64_MAX << (WORD_SIZE-(x))) : 0)

extern char *optarg; //this shouldn't need to be here?
extern int optind, opterr, optopt;

bool init_line(line* a_line);
bool init_set(set* a_set, uint64_t e);
bool init_cache(cache* a_cache, uint64_t s, uint64_t e, uint64_t b);
bool does_set_contain(uint64_t wanted_tag, set* a_set);
uint64_t extract_set_bits(uint64_t address, uint64_t set_bits_num, uint64_t tag_bits_num);
uint64_t extract_tag_bits(uint64_t address, uint64_t set_bits_num, uint64_t tag_bits_num);
bool does_cache_contain(uint64_t address, cache* a_cache);
int cache_block(cache* a_cache, uint64_t address, uint64_t time);

bool get_params(int argc, char **argv, uint64_t* s, uint64_t* e, \
                uint64_t* b, char* file_string, bool* verbose, bool* help);
void print_help_message(void);
#endif