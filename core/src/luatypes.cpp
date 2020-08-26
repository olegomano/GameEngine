#include "../include/luaContext.h"
#include <string>
#include <vector>

template<>
int lua::push_struct<int>(lua_State* lua, const int& data){
    lua_pushinteger(lua,data);
    return 1;
}

template<>
int lua::push_struct<uint32_t>(lua_State* lua, const uint32_t& data){
    lua_pushinteger(lua,data);
    return 1;
}

template<>
int lua::push_struct<uint64_t>(lua_State* lua, const uint64_t& data){
  lua_pushinteger(lua,data);
  return 1;
}

template<>
int lua::push_struct<long>(lua_State* lua, const long& data){
  lua_pushinteger(lua,data);
  return 1;
}

template<>
int lua::push_struct<float>(lua_State* lua, const float& data){
    lua_pushnumber(lua,data);
    return 1;
}

template<>
int lua::push_struct<void*>(lua_State* lua, void* const & data){
    lua_pushlightuserdata(lua,data);
    return 1;
}

template<>
int lua::push_struct<double>(lua_State* lua, const double& data){
    lua_pushnumber(lua,data);
    return 1;
}

template<>
int lua::push_struct<std::string>(lua_State* lua, const std::string& data){
    lua_pushlstring(lua,data.c_str(),data.size());
    return 1;
}

template<>
void lua::read_struct<uint32_t>(lua_State* lua, uint32_t& out){
    out = lua_tointeger(lua,-1);
}

template<>
void lua::read_struct<int>(lua_State* lua, int& out){
    out = lua_tointeger(lua,-1);
}

template<>
void lua::read_struct<void*>(lua_State* lua, void*& out){
    out = lua_touserdata(lua,-1);
}

template<>
void lua::read_struct<float>(lua_State* lua, float& out){
    out = lua_tonumber(lua,-1);
}


template<>
void lua::read_struct<double>(lua_State* lua, double& out){
    out = lua_tonumber(lua,-1);
}


template<>
void lua::read_struct<std::string>(lua_State* lua, std::string& out){
    out = std::string(lua_tostring(lua,-1));
}

template<>
void lua::read_struct<std::vector<std::string>>(lua_State* lua, std::vector<std::string>& out){
  lua_pushnil(lua);
  while(lua_next(lua,-2)){
    int          keyType   = lua_type(lua, -2);
    std::string  value     = lua_tostring(lua,-1);
    lua_pop(lua,1);
    out.push_back(value);
  }  
}


template<>
int lua::push_struct<lua::FlatTableItem>(lua_State* lua, const lua::FlatTableItem& item){
  item.visit([&](auto args...){
    lua::push_struct(lua,args);
  });
  return 1;
}


template<>
int lua::push_struct<lua::FlatTable>(lua_State* lua, const lua::FlatTable& table){
  for(lua::FlatTable::const_iterator iter = table.begin(); iter != table.end(); ++iter){
    const std::string& key = iter->first;
    const lua::FlatTableItem& value = iter->second;
    lua::push_struct(lua,key);
    lua::push_struct(lua,value);
    lua_settable(lua,-3);
  }  
  return 1;
}

template<>
void lua::read_struct<lua::FlatTable>(lua_State* lua, lua::FlatTable& out){
  lua_pushnil(lua);
  while(lua_next(lua,-2)){
    int valueType = lua_type(lua,-1);
    std::string keyName = lua_tostring(lua,-2);
    switch(valueType){
    case LUA_TFUNCTION:
      out[keyName] = (uint32_t)luaL_ref(lua,LUA_REGISTRYINDEX);
      break;
    case LUA_TSTRING:
      out[keyName] = lua_tostring(lua,-1);
      lua_pop(lua,1);
      break;
    case LUA_TNUMBER:   
      out[keyName] = lua_tonumber(lua,-1);
      lua_pop(lua,1);
      break;
    case LUA_TBOOLEAN:
      lua_pop(lua,1);
      break;
    default:
      lua_pop(lua,1);
      break;
    }
  }
  lua_pop(lua,1);
}


void lua::stackDump (lua_State *L) {
      printf("\n=============\n");  /* end the listing */
      int i;
      int top = lua_gettop(L);
      for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {
    
          case LUA_TSTRING:  /* strings */
            printf("`%s'", lua_tostring(L, i));
            break;
    
          case LUA_TBOOLEAN:  /* booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;
    
          case LUA_TNUMBER:  /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;
    
          default:  /* other values */
            printf("%s", lua_typename(L, t));
            break;
        }
        printf("  ");  /* put a separator */
    }
    printf("\n=============\n");  /* end the listing */
}


