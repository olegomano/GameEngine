#include "types.h"




template<>
int lua::push_struct<render::scene::Entity>(lua_State* lua, const Entity& e){  
  return lua::LuaRef::Type::TABLE;
}

template<>
void lua::read_struct<render::scene::Entity>(lua_State* lua, const Entity& e){

}

template<>
int lua::push_struct<render::ICamera>(lua_State* lua, const ICamera& a){  
  return lua::LuaRef::Type::TABLE;
}

template<>
void lua::read_struct<render::ICamera>(lua_State* lua, ICamera& a){

}

template<>
void lua::push_struct<Transform>(lua_State* lua, const Transform& t){

}

template<>
int lua::push_struct<Transform>(lua_State* lua, Transform& out){

}

template<>
int lua::push_struct<Drawable>(lua_State* lua, const Drawable& d){
  lua_pushstring(lua,Drawable::LuaX);
  lua_pushnumber(lua,d.position.positionX());
  lua_settable(lua,-3);
  lua_pushstring(lua,Drawable::LuaY);
  lua_pushnumber(lua,d.position.positionY());
  lua_settable(lua,-3);
  lua_pushstring(lua,Drawable::LuaZ);
  lua_pushnumber(lua,d.position.positionZ());
  lua_settable(lua,-3);
  return lua::LuaRef::Type::TABLE;
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
  lua_pop(lua,1);
}



