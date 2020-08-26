#include "types.h"
#include <luaContext.h>
#include <transform.h>
#include <entity.h>


template<>
int lua::push_struct<render::scene::Entity>(lua_State* lua, const render::scene::Entity& e){  
  return 1;
}

template<>
void lua::read_struct<render::scene::Entity>(lua_State* lua, render::scene::Entity& e){
  
}

template<>
int lua::push_struct<render::ICamera*>(lua_State* lua, render::ICamera* const & a){  
  return 1;
}

template<>
void lua::read_struct<render::ICamera*>(lua_State* lua, render::ICamera*& a){

}

template<>
int lua::push_struct<Transform>(lua_State* lua, const Transform& t){
  lua::FlatTable table;
  table["x"] = t.positionX();
  table["y"] = t.positionY();
  table["z"] = t.positionZ();
  return lua::push_struct(lua,table);
}

template<>
void lua::read_struct<Transform>(lua_State* lua, Transform& out){
  lua::FlatTable table;
  lua::read_struct(lua,table);
  out.setPositionX(table["x"]);
  out.setPositionY(table["y"]);
  out.setPositionZ(table["z"]);
}
