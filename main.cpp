#include <iostream>
#include <string>
#include "Cache.h"
#include "Simulation.h"


int main(int argc, char *argv[]){
        
    int *block_size = new int[1];
    int *number_lines = new int[1];
    int *associativity = new int[1];
    int *replacement_policy =  new int[1];
    
    std::string address;
    std::string operation;
    std::string offset;
    std::string set;
    std::string tag;
    
    Simulation simulation;
        
    simulation.read_cache_specfication(argv[1], block_size, number_lines, associativity, replacement_policy);
    
    Cache cache(*block_size, *number_lines, *associativity, *replacement_policy);

    cache.prepare_cache();


    simulation.read_trace_file(argv[2], *block_size, *number_lines, *associativity, *replacement_policy, cache);


    simulation.create_output_file(argv[3], cache.read_hits, cache.read_misses, cache.write_hits, cache.write_misses, cache.accesses);

    return 0;
}