#include "types.h"


namespace lua{
template<>
int push_struct<GameComponent>(lua_State* lua, const GameComponent& component){
  LuaRef ref = component.luaRef();
  return lua::push_struct(lua,ref);
} 
};


template<>
lua::LuaRef lua::create_table<Drawable>(lua::Script* s, const Drawable& d){
  lua::FlatTable dtable;
  dtable[std::string(Drawable::LuaX)] = d.position.positionX();
  dtable[std::string(Drawable::LuaY)] = d.position.positionY();
  dtable[std::string(Drawable::LuaZ)] = d.position.positionZ();
  return create_table(s,dtable);
}

template<>
void lua::read_struct<Drawable>(lua_State* lua, Drawable& out){ 
  lua_pushnil(lua);
  while(lua_next(lua,-2)){
    size_t keyLen;
    const char* key = lua_tolstring(lua,-2,&keyLen);
    const std::string keyString(key,keyLen);
    if(keyString == std::string(Drawable::LuaX)){
      float x;
      lua::read_struct(lua,x);
      out.position.setPositionX(x);
    }else if(keyString == std::string(Drawable::LuaY)){
      float y;
      lua::read_struct(lua,y);
      out.position.setPositionY(y);
    }else if(keyString == std::string(Drawable::LuaZ)){
      float z;
      lua::read_struct(lua,z);
      out.position.setPositionZ(z);
    }
    lua_pop(lua,1);
  }
}



