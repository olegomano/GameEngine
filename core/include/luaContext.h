#ifndef _CORE_LUA_CONTEXT_H_
#define _CORE_LUA_CONTEXT_H_
#include <variant>
#include <map>
#include "log.h"
#include <cinttypes>
#include <string>
#include <vector>
#include <assert.h>
extern "C"{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

namespace lua{

typedef std::variant<double,std::string> _V_FlatTableItem; 
class FlatTableItem : public _V_FlatTableItem{
public:
  using _V_FlatTableItem::_V_FlatTableItem;
  using _V_FlatTableItem::operator=;
  operator int(){
    return std::get<double>(*this);
  }

  operator uint64_t(){
    return std::get<double>(*this);
  }

  operator uint32_t(){
    return std::get<double>(*this);
  }

  operator double(){
    return std::get<double>(*this);
  }

  operator float(){
    return std::get<double>(*this);
  }

  operator std::string(){
    return std::get<std::string>(*this);
  }
};

void stackDump (lua_State *L);
typedef std::map<std::string,FlatTableItem> FlatTable;

template<typename T>
int push_struct(lua_State* lua, const T& data);

template<typename T>
void read_struct(lua_State* lua, T& out);

template<typename T>
void set_table_item(lua_State* lua, const std::string& name, const T& value){
  push_struct(lua,name);
  push_struct(lua,value);
  lua_settable(lua,-3);
}

template<typename T>
void read_global(lua_State* lua,const std::string name, T& out){
  lua_getglobal(lua,name.c_str());
  read_struct(lua,out);
  lua_pop(lua,1);
}

} //NAMESPACE lua

namespace std{
  template<>
  struct variant_size<lua::FlatTableItem> : variant_size<lua::_V_FlatTableItem>{};

  template<size_t I>
  struct variant_alternative<I,lua::FlatTableItem> : variant_alternative<I,lua::_V_FlatTableItem>{};
};



namespace lua{
class LuaContext;

#define LOG_TAG "LuaContext"



class LuaVar{
public:
  friend std::ostream& operator<<(std::ostream& out, const LuaVar& var){
    out << (void*)(var.m_context) << " " << var.m_ref;
    return out;
  }
  LuaVar(){}
  LuaVar(const LuaVar& other){
    m_context = other.m_context;
    m_ref     = other.m_ref;
  }
  LuaVar(const LuaVar&& other){
    m_context = other.m_context;
    m_ref     = other.m_ref;
  }
  
  LuaVar& operator=(const LuaVar& other){
    m_ref = other.m_ref;
    m_context = other.m_context;
    return *this;
  }

  LuaVar(LuaContext* context,uint32_t ref){
    m_context = context;
    m_ref     = ref;
  }

  uint32_t index() const {return m_ref;}

  template<typename T>
  void write(const T& data);

  template<typename T>
  void read(T& out);
private:
  LuaContext* m_context = nullptr;
  uint32_t    m_ref     = -1;
};

class LuaContext{
public:
  static constexpr char* _INDEX_TABLE = "_INDEX_TABLE";
  static constexpr char* _LUA_ID      = "_ID";
  
  void init(uint32_t id){
    m_id = id;  
    m_lua = luaL_newstate();
    luaL_openlibs(m_lua);    
    
    createGlobal(std::string(_LUA_ID),id);
    lua_newtable(m_lua);
    lua_setglobal(m_lua,_INDEX_TABLE);
  }

  bool loadBuffer(const uint8_t* buffer, uint32_t len,std::ostream& out = std::cout){
    int res = luaL_loadbuffer(m_lua,(const char*)buffer,len,"");
    
    switch(res){
    case 0:
      if(lua_pcall(m_lua,0,0,0)){
        out << lua_tostring(m_lua,-1) << std::endl;
        return false;
      }
      return true;
    case LUA_ERRSYNTAX:
    case LUA_ERRMEM:
    default:
      out << lua_tostring(m_lua,-1) << std::endl;
      lua_pop(m_lua,1);
      return false;
    }
  }

  void registerFunction(const std::string& name, lua_CFunction function){
    lua_register(m_lua,name.c_str(),function);
  }

  template<typename T>
  void createGlobal(const std::string& name, const T& value);
 
  template<typename T>
  void readGlobal(const std::string& name, T& out);

  template<typename T>
  LuaVar createVar(const T& data);

  LuaVar declareVar(){
    uint32_t index = m_allVars.size() + 1;
    LuaVar var = LuaVar(this,index);

    lua_getglobal(m_lua,_INDEX_TABLE);
    lua_pushnumber(m_lua,index);
    lua_newtable(m_lua);
    lua_settable(m_lua,-3);

    lua_pop(m_lua,1);
    m_allVars.push_back(var);
    return var;
  }

  template<typename T>
  void readVar(LuaVar var,T& out);

  template<typename T>
  void writeVar(LuaVar var, const T& out);
  
  void loadVar(LuaVar var){
    cprint_debug(LOG_TAG) << "loadVar " << var << std::endl;
    lua_getglobal(m_lua,_INDEX_TABLE);
    stackDump(m_lua);
    lua_pushnumber(m_lua,var.index());
    stackDump(m_lua);
    lua_gettable(m_lua,-2);
    stackDump(m_lua);
  }

  uint32_t id() const;

private:
  lua_State*          m_lua;
  uint32_t            m_id;
  std::vector<LuaVar> m_allVars;
};

template<typename T>
LuaVar LuaContext::createVar(const T& out){
  LuaVar var = declareVar();
  writeVar(var,out);
  return var;
}


template<typename T>
void LuaContext::readVar(LuaVar var,T& out){  
  loadVar(var);
  read_struct(m_lua,out);
  lua_pop(m_lua,1);
}

template<typename T>
void LuaContext::writeVar(LuaVar var, const T& data){
  loadVar(var);
  lua::push_struct(m_lua,data);
  lua_pop(m_lua,1);
}

template<typename T>
void LuaContext::createGlobal(const std::string& name, const T& value){
  push_struct(m_lua,value);
  lua_setglobal(m_lua,name.c_str());
}

template<typename T>
void LuaContext::readGlobal(const std::string& name, T& out){
  read_global(m_lua,name,out);
}



template<typename T>
void LuaVar::write(const T& data){
  assert(m_context != nullptr);
  m_context->writeVar(*this,data);
}

template<typename T>
void LuaVar::read(T& out){
  assert(m_context != nullptr);
  m_context->readVar(*this,out);
}

template<typename T>
class MappedObject{
public:
  MappedObject(lua::LuaContext& context,T& ptr) : m_data(ptr) {
    m_luaVar = context.declareVar();
  }

  T* operator ->(){
    return &m_data;
  } 
  void read(){
    m_luaVar.read(m_data);  
  }

  void write(){
    m_luaVar.write(m_data);
  }
private:
  T&               m_data;
  lua::LuaVar      m_luaVar;
};

}
#undef LOG_TAG
#endif
