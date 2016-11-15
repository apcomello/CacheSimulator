#include <vector>
#include "CacheLine.h"

#ifndef CACHESET_H
#define CACHESET_H

class CacheSet {
    public:
        vector<CacheLine> tags;
        CacheSet(int set_lines);
        void prepare_lines();
        int number_set_lines;
};

#endif

