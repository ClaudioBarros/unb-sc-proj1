#include "attack.h"
#include "cipher.h"

#include <cstring>
#include <iostream>
#include <string>
#include <fstream>

static std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

std::string read_file(std::string path)
{
    std::ifstream st(path, std::ios::binary|std::ios::ate);
    if(!st.is_open())
    {
        std::cout << "Unable to open file " << path << "\n\n";
        return "";
    }
    else
    {
        auto size = st.tellg(); 
        std::string str(size, '\0');
        st.seekg(0);
        if(st.read(&str[0], size))
        {
            return str;
        }
        return "";
    }
}

std::string parse_string(std::string s_in)
{
    std::string str;
    for(int i = 0; i < s_in.size(); i++)
    {
        std::string::size_type pos = alphabet.find(s_in[i]);
        if(pos != std::string::npos)    
        {
            str.push_back(s_in[i]);
        }
    }

    return str;
}

std::string recover_characters(std::string& s_og, std::string& s_in)
{
    std::string output;
    int j = 0;
    for (int i = 0; i < s_og.size(); i++) 
    {
        std::string::size_type pos = alphabet.find(s_og[i]);
        if(pos == std::string::npos)    
        {
            output.push_back(s_og[i]);
        }
        else
        {
            if(j < s_in.size())
                output.push_back(s_in[j++]);
        }
    }

    return output;
}

std::string encrypt(std::string key, std::string s_in)
{
    std::string str = parse_string(s_in);

    std::cout << "\n---- Encrypting plaintext...\n\n";

    std::string s_enc = enc(key,str);
    std::string output = recover_characters(s_in, s_enc);

    std::cout << "--Ciphertext: \n\n" << output << "\n\n";

    return output;
}

std::string decrypt(std::string key, std::string s_in)
{
    std::string str = parse_string(s_in);

    std::cout << "\n---- Decrypting ciphertext...\n\n";

    std::string s_dec = dec(key, str);
    std::string output = recover_characters(s_in, s_dec);

    std::cout << "-- Plaintext: \n\n" << output << "\n\n";

    return output;
}


void attack(std::string s_in)
{
    std::string str = parse_string(s_in);

    std::string key = recover_key(str);
    std::cout << "---- The guessed key is: " << key << "\n\n";

    std::string str_dec = dec(key, str);

    std::string output = recover_characters(s_in, str_dec);

    std::cout << "---- Deciphered text using guessed key: \n\n" << output << "\n\n";
}

void get_text(std::string &s_out)
{
    std::cout << "File path not supplied.\n";
    std::cout << "Enter text to encrypt/decrypt/attack: ";
    std::getline(std::cin, s_out); 
    std::cout << "\n";
}

int main(int argc, char* argv[]) 
{
    std::vector<std::string> args;
    if(argc > 1)
    {
        for(int i = 1; i < argc; i++)
        {
            args.push_back(argv[i]);
        }
    }

    if(args.size() > 0) 
    {
        if(args[0] == "-a")
        {
            std::string plaintext;
            if(args.size() < 2)
            {
                get_text(plaintext);
            }
            else
            {
                std::string filepath = args[1];
                plaintext = read_file(filepath);
            }

            attack(plaintext);
            return 0;
        }


        if(args[0] == "-e")
        {
            std::string key = args[1];
            
            std::string plaintext;
            if(args.size() >= 2)
            {
                if(args.size() < 3)
                {
                    get_text(plaintext);
                }
                else
                {
                    std::string filepath = args[2];
                    plaintext = read_file(filepath);
                }
            }
            else
            {
                std::cout << "Key not supplied. Exiting...";
                return 0;
            }

            encrypt(key, plaintext);

            return 0;
        }

        if(args[0] == "-d")
        {
            std::string key = args[1];
            
            std::string plaintext;
            if(args.size() >= 2)
            {
                if(args.size() < 3)
                {
                    get_text(plaintext);
                }
                else
                {
                    std::string filepath = args[2];
                    plaintext = read_file(filepath);
                }
            }
            else
            {
                std::cout << "Key not supplied. Exiting...";
                return 0;
            }

            decrypt(key, plaintext);

            return 0;
        }

        if(args[0] == "-ed")
        {
            std::string key = args[1];
            
            std::string plaintext;
            if(args.size() >= 2)
            {
                if(args.size() < 3)
                {
                    get_text(plaintext);
                }
                else
                {
                    std::string filepath = args[2];
                    plaintext = read_file(filepath);
                }
            }
            else
            {
                std::cout << "Key not supplied. Exiting...";
                return 0;
            }

            std::string s_enc = encrypt(key, plaintext);
            decrypt(key, s_enc);

            return 0;
        }
        else
        {
            std::cout << "No valid options supplied.";
        }
    }
    else
    {
        std::cout << "No options supplied.";
    }

}
