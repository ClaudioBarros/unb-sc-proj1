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

int guess_key_length(std::string s_in);

std::string recover_key(std::string s_in);

