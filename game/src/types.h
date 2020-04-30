#ifndef _TYPES_H_
#define _TYPES_H_
#include "core.h"
#include <vector>
#include <variant>


//core should define some kind of standard pointer type so i can have an array of it here
//IPtr -> Ptr<T>
class Drawable {
public:
  static constexpr const char* const LuaX = "x";
  static constexpr const char* const LuaY = "y";
  static constexpr const char* const LuaZ = "z";
};




#endif
