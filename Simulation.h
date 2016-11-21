#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "CacheLine.h"
#include "CacheSet.h"
#include "Cache.h"

#ifndef SIMULATION_H

#define SIMULATION_H

class Simulation{
    
    public:
        int read_cache_specfication(char* file_name, int *block_size, int *number_lines, int *associativity, int *replacement_policy);

        int calculate_offset(int block_size);

        int calculate_set(int associativity, int number_lines);

        int calculate_tag(int offset_bits, int set_bits);

        int split_address(std::string address, int block_size, int associativity, int number_lines, std::string *offset, std::string *index, std::string *tag);

        std::string decimal_to_binary(std::string address);

        void create_output_file(char* file_name, long int read_hits, long int read_misses, long int write_hits, long int write_misses, long int accesses);

        int read_trace_file(char* file_name, int block_size, int number_lines, int associativity,  int replacement_policy, Cache& cache);
};


#endif
