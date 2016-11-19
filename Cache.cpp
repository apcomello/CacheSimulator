#include <iostream>
#include <string>
#include <vector>
#include "CacheSet.h"
#include "CacheLine.h"
#include "Cache.h"

const int LRU = 1;
const int FIFO = 0;
const int W = 1;
const int R = 0;


Cache::Cache(int size_block, int line_numbers, int number_sets, int policy){
    
    block_size = size_block;
    number_lines = line_numbers;
    associativity = number_sets;
    replacement_policy = policy;
    
    
    write_hits = 0;
    write_misses = 0;
    read_hits = 0;
    read_misses = 0;
    accesses = 0;

    
}

void Cache::prepare_cache(){
    
    int i =0;
    int number_sets = number_lines/associativity;
    
    for (i=0; i< number_sets; i++){
        _sets.push_back(CacheSet(associativity));
        _sets[i].prepare_lines();
    }

}

void Cache::LRU_policy(int set_bits, string lookup_tag){
    rotate(_sets[set_bits].tags.begin(), _sets[set_bits].tags.begin()+1, _sets[set_bits].tags.end());
    _sets[set_bits].tags.back().tag = lookup_tag;
}

void Cache::FIFO_policy(int set_bits, string lookup_tag){
    
    rotate(_sets[set_bits].tags.begin(), _sets[set_bits].tags.begin()+1, _sets[set_bits].tags.end());
    _sets[set_bits].tags.back().tag = lookup_tag;
}


int Cache::miss(int set_bits, string lookup_tag, int operation){
    
    size_t i = 0;
        
    if (operation == W){
        write_misses++;
    }
    else if (operation == R){
        read_misses++;
    }
    
    for (i=0; i < _sets[set_bits].tags.size(); i++){
        if (_sets[set_bits].tags[i].tag == "-1"){
            _sets[set_bits].tags[i].tag = lookup_tag;
            for (i=0; i < _sets[set_bits].tags.size(); i++)
            
            return 1;
        }
    }
    
    if (replacement_policy == FIFO)
        FIFO_policy(set_bits, lookup_tag);
    else if (replacement_policy == LRU)
        LRU_policy(set_bits, lookup_tag);
    
    return 1;
}

int Cache::hit(int set_bits, string lookup_tag, int operation, int index){ 
        
    if (operation == W){
        write_hits++;
    }
    else if (operation == R){
        read_hits++;
    }
    
    if (replacement_policy == LRU){
            _sets[set_bits].tags.erase(_sets[set_bits].tags.begin()+index);
            _sets[set_bits].tags.push_back(CacheLine());
            _sets[set_bits].tags.back().tag = lookup_tag;
    }        

    
    return 0;
}


int Cache::lookup(int set_bits, string lookup_tag, int operation){
    
    size_t i =0;
    accesses++;
    
    for (i=0; i < _sets[set_bits].tags.size(); i++){
        if (_sets[set_bits].tags[i].tag == lookup_tag){
            hit(set_bits, lookup_tag, operation, i);
            return 0;
        }
    }

    return miss(set_bits, lookup_tag, operation);
    
}
