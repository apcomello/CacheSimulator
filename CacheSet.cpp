#include "CacheLine.h"
#include "CacheSet.h"

CacheSet::CacheSet(int set_lines){
    
    number_set_lines = set_lines;
}

void CacheSet::prepare_lines(){
    
    int i =0;
    
    for (i=0; i < number_set_lines; i++)
        tags.push_back(CacheLine());
}
