#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>

const int LRU = 1;
const int FIFO = 0;
const int W = 1;
const int R = 0;

using namespace std;

int read_cache_specfication(int *block_size, int *number_lines, int *associativity, int *replacement_policy){
    
    int i =0;
    int position;
    
    
    string proper_buffer;
    string delimiter = " = ";
    string token;
        
    ifstream file;
    
    file.open("cachedesc.bin", ios::binary | ios::in);
    
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
    
    cout << *block_size << endl;
    cout << *number_lines << endl;
    cout << *associativity << endl;
    cout << *replacement_policy << endl;
    
}

int read_trace_file(){
    
    ifstream file;
    string proper_buffer;
    string delimiter = " ";
    string address;
    string operation;
    
    int position;
    
    file.open("input1.bin", ios::binary | ios::in);
    getline(file, proper_buffer);
    
    position = proper_buffer.find(delimiter);

    address = proper_buffer.substr(0, position);
    
    operation = proper_buffer.substr(position + delimiter.length());
    
    cout << address << endl;
    cout << operation << endl;
    
}

int calculate_offset(int block_size){
    return log2(block_size);
}

int calculate_set(int associativity, int number_lines){
    return log2(number_lines/associativity);
}

int calculate_tag(int offset_bits, int set_bits){
    return 64 - offset_bits - set_bits;
}

int split_address(string address, int block_size, int associativity, int number_lines){
    
    string offset;
    string set;
    string tag;
    
    int offset_bits = calculate_offset(block_size);
    int set_bits = calculate_set(associativity, number_lines);
    int tag_bits = calculate_tag(offset_bits, set_bits);
    
    tag = address.substr(0, tag_bits);
    set = address.substr(tag_bits, set_bits);
    offset = address.substr(set_bits+tag_bits, offset_bits);
    
    cout << tag << endl;
    cout << set << endl;
    cout << offset << endl;
    
}

int main(){
    
    int i =0;
    
    int *block_size = new int[1];
    int *number_lines = new int[1];
    int *associativity = new int[1];
    int *replacement_policy =  new int[1];

    
    cout << "Hello, world" << endl;
    cout << "This will be a cache simulator one day." << endl;
    read_cache_specfication(block_size, number_lines, associativity, replacement_policy);
    read_trace_file();
    split_address("0000000101000000011100110111010010001000001101000101100101000100", 64, 8, 512);
    cin >> i;
    
    return 0;
}