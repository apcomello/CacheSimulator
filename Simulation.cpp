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
#include "CacheSet.h"
#include "CacheLine.h"
#include "Cache.h"
#include "Simulation.h"

const int LRU = 1;
const int FIFO = 0;
const int W = 1;
const int R = 0;

using namespace std;

int Simulation::read_cache_specfication(char* file_name, int *block_size, int *number_lines, int *associativity, int *replacement_policy){
    
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

int Simulation::calculate_offset(int block_size){
    return log2(block_size);
}

int Simulation::calculate_set(int associativity, int number_lines){
    return log2(number_lines/associativity);
}

int Simulation::calculate_tag(int offset_bits, int set_bits){
    return 64 - offset_bits;
}

int Simulation::split_address(string address, int block_size, int associativity, int number_lines, string *offset, string *index, string *tag){
        
    int offset_bits = calculate_offset(block_size);
    int set_bits = calculate_set(associativity, number_lines);
    int tag_bits = calculate_tag(offset_bits, set_bits);

    
    *tag = address.substr(0, tag_bits);
    *index = address.substr(tag_bits-set_bits, set_bits);
    *offset = address.substr(set_bits+tag_bits, offset_bits);
    
    return 0;
}

string Simulation::decimal_to_binary(string address){
    
    string binary_address;
    
    binary_address = bitset<64>(strtoll(address.c_str(), NULL, 10)).to_string();
    
    return binary_address;
    
}

void Simulation::create_output_file(char* file_name, long int read_hits, long int read_misses, long int write_hits, long int write_misses, long int accesses){
    
    ofstream output_file;
    
    output_file.open(file_name);
    
    output_file << "Access count: " << accesses;
    output_file << "\nWrite hits: " << write_hits;
    output_file << "\nWrite misses: " << write_misses;
    output_file << "\nRead hits: " << read_hits;
    output_file << "\nRead misses: " << read_misses;
    
    output_file.close();
}

int Simulation::read_trace_file(char* file_name, int block_size, int number_lines, int associativity,  int replacement_policy, Cache& cache){
        
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