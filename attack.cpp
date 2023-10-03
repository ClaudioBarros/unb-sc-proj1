#include "attack.h"
#include <vector>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <map>
#include <unordered_map>
#include <string>
#include <numeric>
#include <iterator>
#include "cipher.h"
#include <iostream>

static std::map<char, float> english_frequency = {
    {'a', 8.167f}, {'b',1.492f}, {'c',2.782f}, {'d',4.253f}, {'e',12.702f}, {'f',2.228f}, {'g', 2.015f},
    {'h', 6.094f}, {'i',6.966f}, {'j',0.253f}, {'k',1.772f}, {'l',4.025f}, {'m',2.406f}, {'n', 6.749f},
    {'o', 7.507f}, {'p',1.929f}, {'q',0.095f}, {'r',5.987f}, {'s',6.327f}, {'t',9.056f}, {'u', 2.758f},
    {'v', 0.978f}, {'w',2.360f}, {'x',0.250f}, {'y',1.974f}, {'z',0.074f}};

static std::map<char, int> letter_index= {
    {'a', 0}, {'b',1}, {'c',2}, {'d',3}, {'e',4}, {'f',5}, {'g', 6},
    {'h', 7}, {'i',8}, {'j',9}, {'k',10}, {'l',11}, {'m',12}, {'n', 13},
    {'o', 14}, {'p',15}, {'q',16}, {'r',17}, {'s',18}, {'t',19}, {'u', 20},
    {'v', 21}, {'w',22}, {'x',23}, {'y',24}, {'z',25}};

static std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

static double ic_random = 0.038466;
static double ic_english = 0.065;

static double ic_portuguese = 0.0745;

static bool is_english = true;

bool compTrigrams(Trigram& t1, Trigram& t2)
{
    return t1.frequency > t2.frequency;
}

bool compFactors(std::pair<int, int> p1, std::pair<int, int> p2)
{
    return (p1.second == p2.second) ? (p1.first > p2.first) : (p1.second > p2.second);
}

//brute forcing it for now
std::vector<int> get_factors(int n)
{
    std::vector<int> vec;
    for(int i = 2; i <= n; i++)
    {
        if(n%i == 0) vec.push_back(i);
    }
    return vec;
}

int get_letter_frequency(const char c, std::string str)
{
    return (int) std::count(str.begin(), str.end(), c);
}

std::vector<std::string> get_cosets(std::string s_in, int len)
{
    if((s_in.size() % len) != 0)
        s_in.resize(s_in.size() + len - (s_in.size() % len));

    int n = s_in.size()/len;

    std::vector<std::string> v(len, "");

    int j = 0;
    for(int i = 0; i < s_in.size(); i+=len)
    {
        for(int shift = 0; shift < len; shift++)
        {
            v[shift].append(1, s_in[i+shift]);
        }
    }

    return v;
}

float get_ic(std::string s_in)
{
    double N = (double) s_in.size();
    double a = 1.0/(N * (N - 1.0));

    double acc = 0.0;
     
    for(int i = 0; i < 26; i++)
    {
        double f = (double) get_letter_frequency(alphabet[i], s_in);
        acc += f * (f - 1.0);
    }

    return a * acc;
}

std::vector<std::pair<int,int>> kasiski(std::string s_in) 
{   
    std::vector<Trigram> trigrams;
    std::unordered_map<std::string, int> added; //{trigram, vector index}
    
    for(int i = 0; i <= (s_in.size()-3); i++)
    {
        std::string substr = s_in.substr(i, 3);

        auto it = added.find(substr); 
        if(it == added.end())
        {
            trigrams.push_back(Trigram{substr, 1, {i}});
            added.insert({substr, trigrams.size()-1});
        }
        else
        {
            trigrams[it->second].frequency++;
            trigrams[it->second].indices.push_back(i);
        }
    }
    std::sort(trigrams.begin(), trigrams.end(), compTrigrams);

    //calculate distances and get factors:
    std::vector<std::pair<int, int>> factor_freq; //{factor, frequency}
    std::unordered_map<int, int> added_factors; //factor index

    // std::vector<std::vector<int>> factors;

    for(int i = 0; i < trigrams.size(); i++)
    {
        if(trigrams[i].frequency < 2)
        {
            continue;
        }

        for(int j = 0; j < trigrams[i].indices.size()-1; j++) 
        {
            int a = trigrams[i].indices[j+1];
            int b = trigrams[i].indices[j];
            trigrams[i].distances.push_back(a-b);
            // factors.push_back(get_factors(a-b));

            std::vector<int> factors = get_factors(a-b);

            for(int f = 0; f < factors.size(); f++)
            {
                auto iter = added_factors.find(factors[f]);
                if(iter == added_factors.end())
                {
                    factor_freq.push_back({factors[f], 1});
                    added_factors.insert({factors[f], factor_freq.size()-1});
                }
                else
                {
                    factor_freq[iter->second].second++;
                }
            }
        }
    }

    // return the factor (greater than or equal to 3) with the greatest frequency
    std::sort(factor_freq.begin(), factor_freq.end(), compFactors);

    return factor_freq;
}

bool comp_length(std::tuple<int,int,double> &a, 
                       std::tuple<int,int,double> &b)
{
    if(std::get<1>(a) == std::get<1>(b))
        return (std::get<2>(a) > std::get<2>(b));
    return (std::get<1>(a) > std::get<1>(b));
}

int guess_key_length(std::string s_in)
{
    std::vector<std::pair<int,int>> vec_factors = kasiski(s_in); 

    std::vector<std::tuple<int, int, double>> vec_ics;

    for(auto f : vec_factors)
    {       
        double acc = 0.0;
        std::vector<std::string> v = get_cosets(s_in, f.first);

        for(int i = 0; i < v.size(); i++)
        {
            acc += get_ic(v[i]);
        }

        double avg = acc/(double)v.size();

        //if avg is closer to random ic than to english ic, ignore value
        if(fabs(ic_english - avg) > fabs(ic_random - avg))
        {
            continue;
        }
        
        for(int i = 0; i < vec_factors.size(); i++)
        {
            if(vec_factors[i].first == f.first)
            {
                vec_ics.push_back({f.first, vec_factors[i].second, avg});
            }
        }

    }
    
    std::sort(vec_ics.begin(), vec_ics.end(), comp_length);

    return std::get<0>(vec_ics[0]);
}



double chi_squared(std::string s_in)
{
    double size = (double) s_in.size();

    //relative frequency 
    std::vector<double> freq(26, 0.0);

    double ans = 0.0;
    for(int i = 0; i < 26; i++)
    {
        double f = (double) get_letter_frequency(alphabet[i], s_in);
        freq[i] = f / size * 100.0;

        double f_sqrd = pow(freq[i] - english_frequency[alphabet[i]], 2);
     
        double a =  f_sqrd/english_frequency[alphabet[i]];

        ans += a;
    }
    return ans;
}

bool is_portuguese(std::string s_in)
{
    std::string pt_letters = "ãáàâéêíóõôúçä";

    bool found = false;
    for(int i = 0; i < pt_letters.size(); i++)
    {
        if(s_in.find(pt_letters[i]) != std::string::npos) 
        {
            found = true;
        }
    }

    return found;
}

double str_freq_analysis(std::string s_in)
{

    double size = (double) s_in.size();
    double acc = 0.0;
    for(int i = 0; i < 26; i++)
    {
        double freq = 
            (double) get_letter_frequency(alphabet[i], s_in)/size;
        double expected = english_frequency[alphabet[i]]/100.0;
        
        acc += freq * expected; 
    }

    if(is_english) return fabs(acc - ic_english);

    return fabs(acc - ic_portuguese);
}


std::string recover_key(std::string s_in)
{
    int len = guess_key_length(s_in);

    std::cout << "The guessed key length is: " << len << "\n\n";
    
    std::vector<std::string> cosets = get_cosets(s_in, len);
    std::vector<std::vector<double>> delta_vec(len, std::vector<double>(26, 0.0));

    for(int i = 0; i < len; i++)
    {
        for(int j = 0; j < 26; j++)
        {
            delta_vec[i][j] = str_freq_analysis(dec(std::string(1, alphabet[j]), cosets[i]));
        }
    }

    std::vector<int> min_idxs(len, 0);
    for(int i = 0; i < len; i++)
    {
        double min = 0.0;
        for(int j = 0; j < 26; j++ )
        {
            if(delta_vec[i][j] < min || j == 0) 
            {
                min = delta_vec[i][j];
                min_idxs[i] = j;
            }
        } 
    }

    std::string key;
    for(int i = 0; i < len; i++)
    {
        key.append(std::string(1, alphabet[min_idxs[i]]));
    }

    return key;
    
}








