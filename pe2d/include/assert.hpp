#pragma once

#ifdef DEBUG_MODE
#include <cassert>
#include <iostream>
#define ASSERT(condition, message)                                             \
  do {                                                                         \
    if (!(condition)) {                                                        \
      std::cerr << "[ASSERT FAILED] " << (message) << '\n';                    \
      assert(condition);                                                       \
    }                                                                          \
  } while (0)
#else
#define ASSERT(condition, message) ((void)0)
#endif