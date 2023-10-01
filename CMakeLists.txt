# CMakeList.txt : CMake project for idj-penguin, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("vigenere-cypher")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


#set compiler preprocessor definitions
add_compile_definitions(PROJECT_DIR=${CMAKE_SOURCE_DIR})

# Add source to this project's executable.

set(SRC_FILES
    "cypher.cpp"
    )

add_executable (cypher ${SRC_FILES})

