#include "csim.h"
#include "testing_csim.h"

#define MAX_FILE_STRING_SIZE 512
//#define DEBUG
static int set_add_block(uint64_t wanted_tag, set* a_set, uint64_t time);

bool init_line(line* a_line) {
    a_line->valid_bit = 0;
    a_line->tag = 0;
    a_line->last_used = 0;
    return 0;
}
bool init_set(set* a_set, uint64_t e) {
    a_set->lines = malloc(e * sizeof(line));
    a_set->num_lines = e;
    for (int64_t i = 0; i< e; i++)
        init_line(&(a_set->lines[i]));
    return 0;
}

bool init_cache(cache* a_cache, uint64_t s, uint64_t e, uint64_t b) {
    if (s > WORD_SIZE) {
        fprintf(stdout, "s is an invalid value %li\n", s);
        return 1;
    }
    if (e > WORD_SIZE) {
        fprintf(stdout, "e is an invalid value %li\n", e);
        return 1;
    }
    if (b > WORD_SIZE) {
        fprintf(stdout, "b is an invalid value %li\n", b);
        return 1;
    }
    a_cache->num_sets = (uint64_t) (1 << s);
    a_cache->tag_bits_num = (uint8_t) (WORD_SIZE - s - b);
    a_cache->sets = (set *) malloc(sizeof(set) * a_cache->num_sets); //CHECK THAT THIS SUCEEDS
    a_cache->set_bits_num = s;
    for (int64_t i = 0; i < a_cache->num_sets; i++)
        init_set(&(a_cache->sets[i]), e);
    return 0;
}
bool does_set_contain(uint64_t wanted_tag, set* a_set) {
    /*returns 1 if contains tag 0 o.w.*/
    for (int64_t i = 0; i< a_set->num_lines; i++)
        if (a_set->lines[i].tag == wanted_tag && a_set->lines[i].valid_bit == 1)
            return 1;
    return 0;
}

uint64_t extract_set_bits(uint64_t address, uint64_t set_bits_num, uint64_t tag_bits_num) {
    //check if shift will be valid
    assert((WORD_SIZE - tag_bits_num - set_bits_num) < (sizeof (LEAD(tag_bits_num + set_bits_num) & TRAIL(WORD_SIZE - tag_bits_num) & address)) * 8 \
        && (WORD_SIZE - tag_bits_num - set_bits_num) >= 0);
    uint64_t set_bits = LEAD(tag_bits_num + set_bits_num);
    set_bits &= TRAIL(WORD_SIZE - tag_bits_num);
    set_bits &= address;
    set_bits >>= (WORD_SIZE - tag_bits_num - set_bits_num);
    //check that the set_bits value is in the correct range
    assert(set_bits >= 0 && set_bits < (uint64_t) 1 << set_bits_num);
    return set_bits;
}

uint64_t extract_tag_bits(uint64_t address, uint64_t set_bits_num, uint64_t tag_bits_num) {
    //check if shift will be valid
    assert((WORD_SIZE - tag_bits_num < (sizeof LEAD(tag_bits_num & address)) * 8 \
        && (WORD_SIZE - tag_bits_num >= 0)));
    //uint64_t tag_bits = (LEAD(tag_bits_num & address) >> (WORD_SIZE - tag_bits_num));
    uint64_t tag_bits = LEAD(tag_bits_num);
    tag_bits &= address;
    tag_bits >>= WORD_SIZE - tag_bits_num;
    //check that the tag_bits value is in the correct range
    assert(tag_bits >= 0 && tag_bits < (uint64_t) 1 << tag_bits_num);
    return tag_bits;
}
bool does_cache_contain(uint64_t address, cache* a_cache) {
     /*returns 1 if contains tag 0 o.w.*/
    uint64_t set_num =  extract_set_bits(address, a_cache->set_bits_num, a_cache->tag_bits_num);
    uint64_t wanted_tag = extract_tag_bits(address, a_cache->set_bits_num, a_cache->tag_bits_num);
    //need to check all sets the address may encompass eventually

    return does_set_contain(wanted_tag, &(a_cache->sets[set_num]));
}
int cache_block(cache* a_cache, uint64_t address, uint64_t time) {
    /* returns 0 if hit, 1 if miss, 2 if miss evict*/
    uint64_t set_num =  extract_set_bits(address, a_cache->set_bits_num, a_cache->tag_bits_num);
    uint64_t wanted_tag = extract_tag_bits(address, a_cache->set_bits_num, a_cache->tag_bits_num);
    //need to check all sets the address may encompass eventually

    return set_add_block(wanted_tag, &(a_cache->sets[set_num]), time);
}

int set_add_block(uint64_t wanted_tag, set* a_set, uint64_t time) {
    bool eviction;
    /*returns 0 if hit, 1 if miss*/
    int64_t oldest_index = 0; 
    /*this is fine because the 0th line will only be evicted due to this start if every line
    was last accessed at the current time
    */
    int64_t oldest_age = time;
    for (int64_t i = 0; i< a_set->num_lines; i++) {
        if (wanted_tag == a_set->lines[i].tag && a_set->lines[i].valid_bit == 1) { //if hit
            a_set->lines[i].last_used = time; //update the time the block was accessed
            return 0;
        }
        else if (a_set->lines[i].last_used < oldest_age) { //keep track of the oldest line throughout this loop
            oldest_index = i;
            oldest_age = a_set->lines[i].last_used;
        }
    }
    //we know we have a miss if we reach here so we perform an eviction
    if (a_set->lines[oldest_index].valid_bit)
        eviction = true;
    else
        eviction = false;
    //perform eviction
    a_set->lines[oldest_index].tag = wanted_tag;
    a_set->lines[oldest_index].last_used = time;
    a_set->lines[oldest_index].valid_bit = 1;
    if (eviction)
        return 2;
    return 1;
}

bool get_params(int argc, char **argv, uint64_t* s, uint64_t* e, \
                uint64_t* b, char* file_string, bool* verbose, bool* help) {
    //opterr = 0; //prevents getopt from printing error messages
    char c;
    while ((c = getopt(argc, argv, "hvs:E:b:t:")) != -1)
        switch(c)
        {
        case 'h':
            *help = true;
            break;
        case 'v':
            *verbose = true;
            break;
        case 's':
            *s = atoi(optarg);
            break;
        case 'E':
            *e = atoi(optarg);
            break;
        case 'b':
            *b = atoi(optarg);
            break;
        case 't':
            strcpy(file_string, optarg);
            break;
        case '?':
            if (optopt == 'h' || optopt == 'v' || optopt == 's' \
             || optopt == 'E' || optopt == 'b' || optopt == 't')
                fprintf(stderr, "Option -%c requires an arguement.\n", optopt);
            else
                fprintf(stderr, "Unkown option character '\\x%x'.\n", optopt);
            return 1;
        default:
            abort();
        }
    return 0;
}
void print_help_message(void) {
    printf("help message\n");
}
int main(int argc, char **argv) {
    cache a_cache;
    uint64_t time = 1; //used to keep track of the current time so we can do LRU eviction
    uint64_t s, e, b;
    char file_string[MAX_FILE_STRING_SIZE];
    bool verbose = 0, help = 0;
    FILE* trace_file;

    uint64_t address, size;
    char operation;

    uint64_t hits = 0, misses = 0, evictions = 0;
    if (argc < 9) {
        fprintf(stderr, "argc is %i but should be >= 9\n", argc);
        print_help_message();
        return 1;
    }
    if (get_params(argc, argv, &s, &e, &b, file_string, &verbose, &help))
        return 1;
    
    #ifdef DEBUG
    printf("s: %lu E: %lu b: %lu t: %s h: %i v: %i\n", s, e, b, file_string, help, verbose);
    #endif
    trace_file = fopen(file_string, "r");

    init_cache(&a_cache, s, e, b);
    while (fscanf(trace_file, " %c %lx,%lu", &operation, &address, &size) == 3) { //this stopping conditon is bad
        uint64_t a;
        if (operation == 'I')
            continue;
        else if (operation == 'M') //there should be another contingency for M
            a = 2;
        else
            a = 1;
        if (verbose) {
            printf("%c %lx,%lu ", operation, address, size);
            #ifdef DEBUG
            cache_print(&a_cache);
            #endif
        }
        while (a > 0) {
            uint8_t b = cache_block(&a_cache, address, time);
            switch(b) {
            case 0:
                if (verbose) printf("hit ");
                hits++;
                break;
            case 1:
                if (verbose) printf("miss ");
                misses++;
                break;
            case 2:
                if (verbose) printf("miss eviction ");
                misses++;
                evictions++;
                break;
            }
            a--;
            time++;
        }
        if (verbose) printf("\n");
    }
    //test_extract_set_bits();
    //test_init_cache();
    printSummary(hits, misses, evictions);
    return 0;
}