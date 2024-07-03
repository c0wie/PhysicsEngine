#pragma once

#include <iostream>

// #define DEBUG

#ifdef DEBUG
    template<typename ...T>
    void Print(T&&... args) 
    {
	    (std::cout << ... << args);
    }
    #define LogCall(...) Print(__VA_ARGS__)
#else
    #define LogCall(...) 
#endif

#define ASSERT(x) errorMessage(__FILE__, __LINE__, x);\
    std::exit(1)

constexpr float PI = 3.1415f;
inline void errorMessage(const char *file, int line, const char *msg)
{
    std::cout << "In file " << file << " at line " << line << "\nERROR: " <<  msg << std::endl;
}

