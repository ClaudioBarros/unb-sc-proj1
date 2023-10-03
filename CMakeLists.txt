# CMakeList.txt : CMake project for idj-penguin, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("vigenere-cipher")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#set compiler preprocessor definitions
add_compile_definitions(PROJECT_DIR=${CMAKE_SOURCE_DIR})

# Add source to this project's executable.

set(SRC_FILES
    "cipher.cpp"
    "attack.cpp"
    "main.cpp"
    )

add_executable (vig ${SRC_FILES})

