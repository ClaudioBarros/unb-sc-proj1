#include "cipher.h"
#include <map>
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>

static std::map<char, int> letter_index= {
    {'a', 0}, {'b',1}, {'c',2}, {'d',3}, {'e',4}, {'f',5}, {'g', 6},
    {'h', 7}, {'i',8}, {'j',9}, {'k',10}, {'l',11}, {'m',12}, {'n', 13},
    {'o', 14}, {'p',15}, {'q',16}, {'r',17}, {'s',18}, {'t',19}, {'u', 20},
    {'v', 21}, {'w',22}, {'x',23}, {'y',24}, {'z',25}};


static std::string alphabet = "abcdefghijklmnopqrstuvwxyz";


std::string enc(const std::string key,
                const std::string s_in)
{
    
    std::string s_out(s_in.size(), 0); //ciphertext

    int k = 0; //key index
    int shift = 0; 
    int base = 0; 
    int k_size = key.size();

    for(int i = 0; i < s_in.size(); i++)
    {
        base = letter_index[s_in[i]];

        shift = letter_index[key[k % k_size]];

        s_out[i] = alphabet[(base + shift) % 26];

        k++;
    }
     
    return s_out;
}

std::string dec(std::string key,
                std::string s_in)
{
    
    std::string s_out(s_in.size(), 0); //ciphertext

    int k = 0; //key index
    int shift = 0; 
    int base = 0; 
    int k_size = key.size();
    int index = 0;

    for(int i = 0; i < s_in.size(); i++)
    {
        base = letter_index[s_in[i]];

        shift = letter_index[key[k % k_size]];

        index = (base < shift) ? ((base - shift) + 26) : (base - shift);
        
        s_out[i] = alphabet[index % 26];

        k++;
    }
     
    return s_out;
}

