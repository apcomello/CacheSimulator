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
#include "CacheLine.h"
#include "CacheSet.h"
#include "Cache.h"
#include "Simulation.h"

const int LRU = 1;
const int FIFO = 0;
const int W = 1;
const int R = 0;

using namespace std;

int main(int argc, char *argv[]){
    

    
    int i =0;
    


    int *block_size = new int[1];
    int *number_lines = new int[1];
    int *associativity = new int[1];
    int *replacement_policy =  new int[1];
    
    string address;
    string operation;
    string offset;
    string set;
    string tag;
    
    Simulation simulation;
        
    simulation.read_cache_specfication(argv[1], block_size, number_lines, associativity, replacement_policy);
    
    Cache cache(*block_size, *number_lines, *associativity, *replacement_policy);


    cout << *block_size << endl;
    cache.prepare_cache();


    simulation.read_trace_file(argv[2], *block_size, *number_lines, *associativity, *replacement_policy, cache);


    simulation.create_output_file(argv[3], cache.read_hits, cache.read_misses, cache.write_hits, cache.write_misses, cache.accesses);

    
    cin >> i;
    
    return 0;
}