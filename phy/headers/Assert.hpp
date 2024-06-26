#pragma once

#include <cassert>
#define DEBUG
#ifdef DEBUG
    #define LogCall(x) std::cout << x << std::endl;
#else
    #define LogCall(x) 
#endif

#define ASSERT(x) errorMessage(__FILE__, __LINE__, x);\
    std::exit(1)

inline void errorMessage(const char *file, int line, const char *msg)
{
    std::cout << "In file " << file << " at line " << line << "\nERROR: " <<  msg << std::endl;
}

