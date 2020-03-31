#ifndef _TYPES_H_
#define _TYPES_H_
#include "core.h"
#include "renderer.h"
#include <vector>
#include <variant>


//core should define some kind of standard pointer type so i can have an array of it here
//IPtr -> Ptr<T>
class Drawable {
public:
  static constexpr char* LuaX = "x";
  static constexpr char* LuaY = "y";
  static constexpr char* LuaZ = "z";
};




#endif
