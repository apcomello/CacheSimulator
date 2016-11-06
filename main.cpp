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

const int LRU = 1;
const int FIFO = 0;
const int W = 1;
const int R = 0;

using namespace std;

/*
    CACHE LINE
*/

class CacheLine {
    public:
        // bool contains_data;
        string tag;
        CacheLine();
};

CacheLine::CacheLine(void){
    
    tag = "-1";
}


/*
    CACHE SET
*/

class CacheSet {
    public:
        vector<CacheLine> tags;
        CacheSet(int set_lines);
        void prepare_lines();
        int number_set_lines;
};

CacheSet::CacheSet(int set_lines){
    
    number_set_lines = set_lines;
}

void CacheSet::prepare_lines(){
    
    int i =0;
    
    for (i=0; i < number_set_lines; i++)
        tags.push_back(CacheLine());
}

/*
    CACHE
*/

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
        // cout << "WRITE MISS" << endl;
        write_misses++;
    }
    else if (operation == R){
        // cout << "READ MISS" << endl;
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
        // cout << "WRITE HIT" << endl;
        write_hits++;
    }
    else if (operation == R){
        // cout << "READ HIT" << endl;
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

/*
    OTHER FUNCTIONS
*/

int read_cache_specfication(char* file_name, int *block_size, int *number_lines, int *associativity, int *replacement_policy){
    
    int i =0;
    int position;
    
    
    string proper_buffer;
    string delimiter = " = ";
    string token;
        
    ifstream file;
    
    file.open(file_name, ios::binary | ios::in);
    
    for(i = 0; i < 4; i++){
        getline(file, proper_buffer);
        position = proper_buffer.find(delimiter);
        
        token = proper_buffer.substr(position + delimiter.length());
        
        if (token.find('\r') == token.length()-1)
            token.erase(token.length()-1, 1);
                
        switch(i){
            case 0:
                istringstream(token) >> *block_size;
                break;
            case 1:
                istringstream(token) >> *number_lines;
                break;
            case 2:
                istringstream(token) >> *associativity;
                break;
            case 3:
                if (token == "LRU")
                    *replacement_policy = LRU;
                else if (token == "FIFO")
                    *replacement_policy = FIFO;
                break;
        }
    }
    return 0;
}

int calculate_offset(int block_size){
    return log2(block_size);
}

int calculate_set(int associativity, int number_lines){
    return log2(number_lines/associativity);
}

int calculate_tag(int offset_bits, int set_bits){
    return 64 - offset_bits;
}

int split_address(string address, int block_size, int associativity, int number_lines, string *offset, string *index, string *tag){
        
    int offset_bits = calculate_offset(block_size);
    int set_bits = calculate_set(associativity, number_lines);
    int tag_bits = calculate_tag(offset_bits, set_bits);

    
    *tag = address.substr(0, tag_bits);
    *index = address.substr(tag_bits-set_bits, set_bits);
    *offset = address.substr(set_bits+tag_bits, offset_bits);
    
    return 0;
}

string decimal_to_binary(string address){
    
    string binary_address;
    
    binary_address = bitset<64>(strtoll(address.c_str(), NULL, 10)).to_string();
    
    return binary_address;
    
}

void create_output_file(char* file_name, long int read_hits, long int read_misses, long int write_hits, long int write_misses, long int accesses){
    
    ofstream output_file;
    
    output_file.open(file_name);
    
    output_file << "Access count: " << accesses;
    output_file << "\nWrite hits: " << write_hits;
    output_file << "\nWrite misses: " << write_misses;
    output_file << "\nRead hits: " << read_hits;
    output_file << "\nRead misses: " << read_misses;
    
    output_file.close();
}

int read_trace_file(char* file_name, int block_size, int number_lines, int associativity,  int replacement_policy, Cache& cache){
    
    ifstream file(file_name);
    string proper_buffer;
    string delimiter = " ";
    
    string address;
    string operation;
    
    string offset;
    string set;
    string tag;
    
    int position;
    
    int set_bits;
    int operation_bit;
    
    string binary_address;
    
    while (true){
        if (getline(file, proper_buffer));{

            position = proper_buffer.find(delimiter);

            address = proper_buffer.substr(0, position);
            
            operation = proper_buffer.substr(position+delimiter.length(), 1);
            
            if (operation == "W")
                operation_bit = W;
            else if (operation == "R")
                operation_bit = R;
            
            binary_address = decimal_to_binary(address);
            split_address(binary_address, block_size, associativity, number_lines, &offset, &set, &tag);   
            set_bits = strtol(set.c_str(), NULL, 2);
                                    
            cache.lookup(set_bits, tag, operation_bit);

            if (file.eof())
                break;
        }
    }
    
    return 0;
    
}

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
            
    read_cache_specfication(argv[1], block_size, number_lines, associativity, replacement_policy);
        
    Cache cache(*block_size, *number_lines, *associativity, *replacement_policy);

    cache.prepare_cache();

    cout << "Hello, world" << endl;
    cout << "This will be a cache simulator one day." << endl;
    
    
    read_trace_file(argv[2], *block_size, *number_lines, *associativity, *replacement_policy, cache);
        
    create_output_file(argv[3], cache.read_hits, cache.read_misses, cache.write_hits, cache.write_misses, cache.accesses);

    
    cin >> i;
    
    return 0;
}