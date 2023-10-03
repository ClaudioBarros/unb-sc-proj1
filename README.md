# unb-sc-proj1

This project implements a Vigenère cipher (poly-alphabetic shift cipher) and algorithms for guessing the size of the key used in the encoding of a cipher, guessing the key used to encode the cipher, and breaking a ciphertext encoded with an unknown key. 

This project was developed as part of the University of Brasilia's **CIC0201 - Computational Secutiry** course taken in the second semester of 2023. 

### Prerequisites

* **C++ Compiler** - needs to support at least the **C++11** standard, i.e. *MSVC*,
*GCC*, *Clang*

## Building 

To build the project using **CMake**, all you need to do, assuming you have **CMake** installed and you're running in a linux machine, is run a similar routine to the the one below:

For a Debug build:
```bash
mkdir build/ && cd build/
cmake -S --build . 
cmake -B./ -S../-DCMAKE_BUILD_TYPE=Debug
make
```
For a Release build:
```bash
mkdir build/ && cd build/
cmake -S --build . 
cmake -B./ -S../-DCMAKE_BUILD_TYPE=Release
make
```

## Running

#### Decoding a file

> \<executable-name\>.exe -d \<key\> \<filepath\>

#### Encoding a file

> \<executable-name\>.exe -e \<key\> \<filepath\>

#### Encoding and decoding a file

> \<executable-name\>.exe -e \<key\> \<filepath\>

#### Breaking a ciphertext

> \<executable-name\>.exe -a \<filepath\>

## Authors

* **Cláudio Barros** - [@claudiobarros](https://github.com/claudiobarros)

## License

[LICENSE](LICENSE) file for details

