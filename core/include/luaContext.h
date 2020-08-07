#ifndef _CORE_LUA_CONTEXT_H_
#define _CORE_LUA_CONTEXT_H_
#include <variant>
#include <map>
#include "log.h"
#include <cinttypes>
#include <string>
#include <vector>
#include <assert.h>
#include <type_traits> 
extern "C"{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

namespace lua{

typedef std::variant<double,std::string,uint32_t> _V_FlatTableItem; 

struct FunctionHandle{
  FunctionHandle(){}
  FunctionHandle(uint32_t t){
    value = t;
  }
  FunctionHandle(const FunctionHandle& o){
    value = o.value;
  }

  operator uint32_t(){
    return value;
  }

  uint32_t value;
};


class FlatTableItem : public _V_FlatTableItem{
public:
  using _V_FlatTableItem::_V_FlatTableItem;
  using _V_FlatTableItem::operator=;
  
  template<typename T>
  void visit(const T& f) const {
    int indx = index();
    if(indx == 0){
      f(std::get<double>(*this) );
    }else if(indx == 1){
      f(std::get<std::string>(*this) ); 
    }else if(indx == 2){
       
    }else{
      assert(false);
    }
  }

  operator FunctionHandle(){
    return FunctionHandle(std::get<uint32_t>(*this));
  }

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

/*
 * gets the item at index in table
 * table is assumed to be on top of the stack
 * leaves table at the top of the stack
 */
template<typename T>
void get_list_item(lua_State* lua, uint32_t index, T& data){
  lua_rawgeti(lua,-1,index);
  read_struct(lua,data);
}

/*
 * assumes table is on top of the stack
 */
template<typename T>
void set_table_item(lua_State* lua, const std::string& name, const T& value){
  push_struct(lua,name);
  push_struct(lua,value);
  lua_settable(lua,-3);
}
  /**
  * assumes stack[-1] is data
  * assumes stack[-2] is table
   * returns the key of the data
  */
inline uint32_t append_list(lua_State* lua, bool empty = false){
  size_t index = lua_rawlen(lua,-2) + 1;  
  lua_rawseti(lua,-2,index);
  if(empty){
    lua_pop(lua,1);
  }
  return index;
}

template<typename T>
constexpr bool is_primitive(){
  if constexpr ( 
         std::is_same<T,std::string>::value 
      || std::is_fundamental<T>::value 
      || std::is_same<T,void*>::value
      ){
    return true;
  }
  return false;
}

template<typename T>
void read_global(lua_State* lua,const std::string& name, T& out){
  cprint_debug("LuaContext") << "Reading Global " << name << std::endl;
  lua_getglobal(lua,name.c_str());
  read_struct(lua,out);
  lua_pop(lua,1);
}

template<typename T>
void write_global(lua_State* lua, const std::string& name, const T& out){
  push_struct(lua,out);
  lua_setglobal(lua,name.c_str());
  //lua_pop(lua,1); 
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
    m_global  = other.m_global;
  }
  
  LuaVar(const LuaVar&& other){
    m_context = other.m_context;
    m_ref     = other.m_ref;
    m_global  = other.m_global;
  }
  
  LuaVar& operator=(const LuaVar& other){
    m_ref = other.m_ref;
    m_context = other.m_context;
    m_global  = other.m_global;
    return *this;
  }

  LuaVar(LuaContext* context,uint32_t ref,bool global){
    m_context = context;
    m_ref     = ref;
    m_global  = global;
  }

  uint32_t index() const {return m_ref;}
 
  bool isGlobal() {return m_global;}

  template<typename T>
  void write(const T& data) const;

  template<typename T>
  void read(T& data) const;
  
  void load() const;
  
  void call() const; 

private:
  LuaContext* m_context = nullptr;
  uint32_t    m_ref     = -1;
  bool        m_global  = false;
};

class LuaContext{
public:
  static constexpr char* _INDEX_TABLE  = "_INDEX_TABLE";
  static constexpr char* _GLOBAL_TABLE = "_GLOBAL_TABLE";
  
 void init(uint32_t id){
    m_id = id;  
    m_lua = luaL_newstate();
    luaL_openlibs(m_lua);

    lua_newtable(m_lua);
    lua_setglobal(m_lua,_INDEX_TABLE);
    lua_newtable(m_lua);
    lua_setglobal(m_lua,_GLOBAL_TABLE);
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

  template<typename T>
  bool loadBufferForResult(const uint8_t* buffer, uint32_t len, T& result, std::ostream& out = std::cout){
    if(!loadBuffer(buffer,len,out)){
      return false;
    }
    read_struct<T>(m_lua,result); 
    return true;
  }

  void registerFunction(const std::string& name, lua_CFunction function){
    lua_register(m_lua,name.c_str(),function);
  }
  
    /*
   *creates type T and puts it at the top of the stack
   */
  template<typename T>
  void allocate(){
    if constexpr (is_primitive<T>()){
      T t = {};
      lua::push_struct(m_lua,t);
    }else{
      lua_newtable(m_lua);
    }
  }
    /*
  * creates a var and adds it to approrite list
  * leaves it at the top of the stack
  */
  template<typename T>
  LuaVar createVar(bool global, const std::string& globalName = ""){
    if(global){
      cprint_debug("Lua") << "Creating global variable " << globalName << std::endl;
    }
    uint32_t indx = -1;
    const std::string& table = global ? _GLOBAL_TABLE : _INDEX_TABLE;
    
    std::cout << "createVar::start" <<std::endl;
    stackDump(m_lua);

    lua_getglobal(m_lua,table.c_str());
    allocate<T>();
    indx = append_list(m_lua);

    LuaVar var = LuaVar(this,indx,global);
 
    if(global){
      lua_rawgeti(m_lua,-1,indx);
      lua_setglobal(m_lua,globalName.c_str());
      m_globalVars.push_back(var);
    }else{
      m_allVars.push_back(var);
    }
    lua_pop(m_lua,1);

    std::cout << "createVar::end " << indx << std::endl;
    stackDump(m_lua);

    return var;
  }

  template<typename T>
  void read(LuaVar var,T& out){
    load(var);
    read_struct(m_lua,out);
  }

  template<typename T>
  void write(LuaVar var, const T& data) const{
    if constexpr (is_primitive<T>()){
      const std::string& table = var.isGlobal() ? _GLOBAL_TABLE : _INDEX_TABLE;
      lua_getglobal(m_lua,table.c_str());
      push_struct(m_lua,data);
      lua_rawseti(m_lua,-2,var.index());
    }else{
      load(var);
      push_struct(m_lua,data);
    }
    lua_pop(m_lua,1);
    stackDump(m_lua);
  }

  /*
   * puts var at top of lua stack
   */
  void load(LuaVar var) const {
    const std::string& table = var.isGlobal() ? _GLOBAL_TABLE : _INDEX_TABLE;
    lua_getglobal(m_lua,table.c_str());
    lua_pushnumber(m_lua,var.index()); //todo replace with get_list_item
    lua_gettable(m_lua,-2); 
    lua_remove(m_lua,-2);
  }
  
  /**
   * puts the function this handle points to at the top of the stack
   */
  void load(const FunctionHandle& handle) const {
    lua_rawgeti(m_lua, LUA_REGISTRYINDEX, handle.value);
  }
  
  /*
   * puts a global with name on top of the stack
   */
  void load(const std::string& globalName){
    lua_getglobal(m_lua,globalName.c_str());
  }


  void call(const FunctionHandle& handle) const {
    load(handle);
    lua_pcall(m_lua,0,0,0);
  }
  
  uint32_t id() const;
  
  lua_State* lua() {return m_lua;}
private:
  lua_State*                   m_lua;
  uint32_t                     m_id;
  std::vector<LuaVar>          m_allVars;
  std::vector<LuaVar>          m_globalVars;
};


template<typename T>
inline void LuaVar::write(const T& data) const{
  assert(m_context != nullptr);
  m_context->write(*this,data);
}

template<typename T>
inline void LuaVar::read(T& data) const {
  assert(m_context != nullptr);
  m_context->read(*this,data);
}
  
inline void LuaVar::load() const {
  assert(m_context != nullptr);
  m_context->load(*this);
};

inline void LuaVar::call() const{
  m_context->load(*this);
  lua_pcall(m_context->lua(),0,0,0);
}; 

}
#undef LOG_TAG
#endif
