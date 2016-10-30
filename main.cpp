#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

const int LRU = 1;
const int FIFO = 0;

using namespace std;

int read_cache_specfication(int *line_size, int *number_lines, int *associativity, int *replacement_policy){
    
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
                istringstream(token) >> *line_size;
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
    
    cout << *line_size << endl;
    cout << *number_lines << endl;
    cout << *associativity << endl;
    cout << *replacement_policy << endl;
    
}

int main(){
    
    int i =0;
    
    int *line_size = new int[1];
    int *number_lines = new int[1];
    int *associativity = new int[1];
    int *replacement_policy =  new int[1];

    
    cout << "Hello, world" << endl;
    cout << "This will be a cache simulator one day." << endl;
    read_cache_specfication(line_size, number_lines, associativity, replacement_policy);
    
    cin >> i;
    
    return 0;
}