#ifndef _LUA_API_H_
#define _LUA_API_H_
#include "context.h"
#include "core.h"
#include "types.h"
#include <map>



std::map<std::string,lua::FlatTableItem> createLuaEntityState(){

}


int LuaApi_createPrimitive(lua_State* lua){
    void* contextPtr = nullptr;
    lua::read_struct<void*>(lua,contextPtr);

    Context* context  = (Context*) contextPtr;
    const render::gl::Drawable d =  context->renderPrims()[render::Primitives::Triangle];

    Drawable& drawable = contextPtr->renderer().createDrawable(d);
    std::map<std::string,lua::FlatTableItem> luaSharedState = createLuaEntityState();
    
    lua::push_struct(lua,luaSharedState);
    return 1;
};



#endif
