#pragma once

namespace ray {
    extern "C" {
        #include "raylib.h"
    }
}
// Raylib defines these as C macros which escape the namespace wrapper above.
// Undef them and re-declare as proper C++ constants so ray::BLACK etc. work normally.
#undef PI
#undef BLACK
#undef RED
#undef GREEN
#undef YELLOW
#undef BLUE
#undef MAGENTA
#undef CYAN
#undef WHITE
namespace ray {
    constexpr double PI      = 3.14159265358979323846;
    constexpr Color  BLACK   = {  0,   0,   0, 255};
    constexpr Color  RED     = {230,  41,  55, 255};
    constexpr Color  GREEN   = {  0, 228,  48, 255};
    constexpr Color  YELLOW  = {253, 249,   0, 255};
    constexpr Color  BLUE    = {  0, 121, 241, 255};
    constexpr Color  MAGENTA = {255,   0, 255, 255};
    constexpr Color  CYAN    = {  0, 255, 255, 255};
    constexpr Color  WHITE   = {255, 255, 255, 255};
}

#include <algorithm>
#include <functional>
#include <string>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <stack>
#include <utility>
#include <chrono>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <float.h>

using namespace std;
using namespace chrono;