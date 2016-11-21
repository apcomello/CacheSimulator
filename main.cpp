#include <iostream>
#include <string>
#include "Cache.h"
#include "Simulation.h"


int main(int argc, char *argv[]){
    
    
    //Basic cache specification
    int *block_size = new int[1];
    int *number_lines = new int[1];
    int *associativity = new int[1];
    int *replacement_policy =  new int[1];

    //Simulation of read/write operations based on the specified cache
    Simulation simulation;
        
    simulation.read_cache_specfication(argv[1], block_size, number_lines, associativity, replacement_policy);
    
    Cache cache(*block_size, *number_lines, *associativity, *replacement_policy);

    cache.prepare_cache();      //Sets up cache structure (sets/lines) according to the specification


    //Reading of the trace file and execution its instructions
    simulation.read_trace_file(argv[2], *block_size, *number_lines, *associativity, *replacement_policy, cache);        
    //Creation of output file with results
    simulation.create_output_file(argv[3], cache.read_hits, cache.read_misses, cache.write_hits, cache.write_misses, cache.accesses);

    return 0;
}