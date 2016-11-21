#include <vector>
#include "CacheLine.h"

#ifndef CACHESET_H
#define CACHESET_H

class CacheSet {
    
    public:
        std::vector<CacheLine> tags;     //Vector of lines inside the set
        CacheSet(int set_lines);
        void prepare_lines();       //Creates the structure of the set
        int number_set_lines;
};

#endif

