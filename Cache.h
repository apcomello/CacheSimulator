#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include "CacheSet.h"


#ifndef CACHE_H

#define CACHE_H



class Cache {
    public:
        
        Cache(int size_block, int line_numbers, int number_sets, int policy);
    
        int block_size;
        int number_lines;
        int associativity;
        int replacement_policy;
        
        vector<CacheSet> _sets;
        
        void prepare_cache(); //Allocs the necessary memory for all blocks in cache
        
        int lookup(int set_bits, string lookup_tag, int operation);
        
        long int write_hits;
        long int write_misses;
        long int read_hits;
        long int read_misses;
        long int accesses;
        
        
    private:
    

        int hit(int set_bits, string lookup_tag, int operation, int index);      //Returns a hit
        int miss(int set_bits, string lookup_tag, int operation);     //Returns a miss
        void LRU_policy(int set_bits, string lookup_tag);
        void FIFO_policy(int set_bits, string lookup_tag);
        
};


#endif

