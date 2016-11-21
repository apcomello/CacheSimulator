#include <string>

#ifndef CACHELINE_H
#define CACHELINE_H

class CacheLine {
    
    public:
        std::string tag;    //Tag of the block stored
        CacheLine();
};

#endif