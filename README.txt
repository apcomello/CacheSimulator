CacheSimulator: A simple cache simulator in the terminal

Project developed for the class INF01113 - Organização de Computadores B (Computer Organization B) 2016/2 at UFRGS - Universidade Federal do Rio Grande do Sul)

The program simulates a cache that uses a 1 byte word and a write back, write allocate policy. The results show how many accesses were made, the number of write hits, read hits, write misses and read misses.

It takes as input a file containing basic cache specifications (size, associativity, replacement policy), a trace file and an output file in which the simulation's result will be written. 

How the program is called in the terminal:
    ./cacheSimulator cachedesc.bin tracefile.bin output.bin