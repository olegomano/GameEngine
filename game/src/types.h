#ifndef _TYPES_H_
#define _TYPES_H_
#include "core.h"
#include "renderer.h"

struct Drawable{
    Transform             position;
    render::gl::VBOAttrib normals;
    render::gl::VBOAttrib vertex;
};

struct LuaEntity{
    lua::LuaRef luaState;
    Transform   transform;
    
    void update();  
};


#endif