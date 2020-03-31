#include "../include/luascript.h"
#include <string>
#include <vector>

template<>
int lua::push_struct<int>(lua_State* lua, const int& data){
    lua_pushinteger(lua,data);
    return lua::LuaRef::NUMBER;
}

template<>
int lua::push_struct<uint32_t>(lua_State* lua, const uint32_t& data){
    lua_pushinteger(lua,data);
    return lua::LuaRef::NUMBER;
}

template<>
int lua::push_struct<uint64_t>(lua_State* lua, const uint64_t& data){
  lua_pushinteger(lua,data);
  return lua::LuaRef::NUMBER;
}

template<>
int lua::push_struct<long>(lua_State* lua, const long& data){
  lua_pushinteger(lua,data);
  return lua::LuaRef::NUMBER;
}

template<>
int lua::push_struct<float>(lua_State* lua, const float& data){
    lua_pushnumber(lua,data);
    return lua::LuaRef::NUMBER;
}

template<>
int lua::push_struct<void*>(lua_State* lua, void* const & data){
    lua_pushlightuserdata(lua,data);
    return lua::LuaRef::PTR;
}

template<>
int lua::push_struct<double>(lua_State* lua, const double& data){
    lua_pushnumber(lua,data);
    return lua::LuaRef::NUMBER;
}

template<>
int lua::push_struct<std::string>(lua_State* lua, const std::string& data){
    lua_pushlstring(lua,data.c_str(),data.size());
    return lua::LuaRef::STRING;
}

template<>
int lua::push_struct<lua::LuaRef>(lua_State* lua, const lua::LuaRef& ref){
    ref.load();
    return ref.refType();
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
void lua::read_struct<lua::FlatTable>(lua_State* lua, lua::FlatTable& out){
  lua_pushnil(lua);
  while(lua_next(lua,-2)){
    int valueType = lua_type(lua,-1);
    std::string keyName = lua_tostring(lua,-2);

    switch(valueType){
    case LUA_TFUNCTION:
      break;
    case LUA_TSTRING:
      out[keyName] = lua_tostring(lua,-1);
      break;
    case LUA_TNUMBER:
      out[keyName] = lua_tonumber(lua,-1);
      break;
    case LUA_TBOOLEAN:
      break;
    default:
      break;
    }
    lua_pop(lua,1);
  }
  lua_pop(lua,1);
}

template<>
lua::LuaRef lua::create_table(lua::Script* script, const std::map<std::string,lua::FlatTableItem>& data){
    lua_createtable(script->luaState(),0,data.size());
    int luaTableRef = luaL_ref(script->luaState(), LUA_REGISTRYINDEX);
    lua_rawgeti(script->luaState(), LUA_REGISTRYINDEX, luaTableRef);

    std::vector<lua::LuaRef> children;
    std::vector<std::string> childNames;

    for(auto iter : data){
        std::visit([script,luaTableRef,&iter,&childNames,&children]
        (auto&& args) mutable {
            lua::LuaRef::Type type = (lua::LuaRef::Type)push_struct(script->luaState(),args);
            lua::LuaRef child(luaL_ref(script->luaState(), LUA_REGISTRYINDEX),script,type);
            
            lua_rawgeti(script->luaState(), LUA_REGISTRYINDEX, child.ref());
            lua_setfield(script->luaState(), -2, iter.first.c_str());

            children.push_back(child);
            childNames.push_back(iter.first);
        },
        iter.second);
    }


    LuaRef tableRef(luaTableRef,script,lua::LuaRef::TABLE);
    script->addRef("DYNAMIC_TABLE",tableRef);


    for(int i = 0; i < children.size(); i++){
        script->addRef(childNames[i],children[i],tableRef.ID());
    }
    return tableRef;
}

