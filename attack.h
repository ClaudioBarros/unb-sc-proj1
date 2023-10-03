#pragma once

#include <vector>
#include <string>

struct Trigram
{
    std::string str;
    int frequency;
    std::vector<int> indices;
    std::vector<int> distances;
};

void set_language(int language); //1-english 2-portuguese
int guess_key_length(std::string s_in);

std::string recover_key(std::string s_in);

