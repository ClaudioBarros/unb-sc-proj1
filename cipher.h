#pragma once    

#include <string>

std::string enc(const std::string key,
                const std::string s_in);

std::string dec(std::string key,
                std::string s_in); 

