#ifndef _LUA_SCRIPT_H_
#define _LUA_SCRIPT_H_
extern "C"{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}
#include <string>
#include <tuple>
#include <set>
#include <map>
#include <iostream>
#include <variant>
#include <assert.h>
#include <vector>
#include "vstack.h"
#include "observable.h"
#include "utils.h"
#include <glm/glm.hpp>
#include "file.h"
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
}

namespace std{
  template<>
  struct variant_size<lua::FlatTableItem> : variant_size<lua::_V_FlatTableItem>{};

  template<size_t I>
  struct variant_alternative<I,lua::FlatTableItem> : variant_alternative<I,lua::_V_FlatTableItem>{};
};


namespace lua{

typedef std::map<std::string,FlatTableItem> FlatTable;
void stackDump (lua_State *L);


class Script;
class Node;

template<typename T>
int push_struct(lua_State* lua, const T& data);

template<typename T>
void read_struct(lua_State* lua, T& out);

template<typename T>
void read_global(lua_State* lua,const std::string name, T& out){
  lua_getglobal(lua,name.c_str());
  read_struct(lua,out);
  lua_pop(lua,1);
}


class LuaRef{
friend  Node;
friend  Script;
friend  std::ostream& operator<<(std::ostream& os, const LuaRef& dt);
public:
    enum Type{
        NUMBER = 0,
        STRING,
        FUNCTION,
        TABLE,
        PTR
    };  

    LuaRef(uint32_t ref,Script* s, Type t){
        id          = -1;
        luaRef      = ref;
        owner       = s;
        type        = t;
    }
    
    LuaRef(const LuaRef& r){
        id          = r.id;
        luaRef      = r.luaRef;
        owner       = r.owner;
        type        = r.type;
        parentId    = r.parentId;
    }
    
    LuaRef(){
        id == -1;
    }

    template<typename T>
    bool read(T& out) const;
    
    template<typename ...T>
    bool call(const T&... args) const;
    
    const LuaRef operator[](const std::string& name) const;

    bool null() const {
        return id == -1;
    }

    std::string name() const;

    uint32_t ref() const {return luaRef;}
    Type refType() const {
        return type;
    }

    void load() const;
    uint32_t ID() const {return id;}
protected:
    uint32_t   luaRef   = -1;
    Script*    owner;
    Type       type;

    uint32_t   id       = -1;    
    uint32_t   parentId = -1;
};


template<typename T>
LuaRef create_table(Script* script, const T& data);

class Script{
public:
  friend  LuaRef;
  friend  std::ostream& operator<<(std::ostream& os, const Script& dt);
  
  enum Source{
    FILE,
    BUFFER
  };

  Script(const core::file::File& file) 
  :m_source(FILE)
  ,m_file(file)
  ,m_buffer(nullptr)
  ,m_bufferSize(-1)
  {
    m_allLuaRefs.reserve(1024);
    m_refChildren.reserve(1024);
    m_refNames.reserve(1024);
  }

  Script(const uint8_t* buffer, uint32_t len)
  :m_source(BUFFER)
  ,m_file(core::file::File())
  ,m_buffer(buffer)
  ,m_bufferSize(len) 
  {
    m_allLuaRefs.reserve(1024);
    m_refChildren.reserve(1024);
    m_refNames.reserve(1024);
  }

  void registerFunction(const std::string& name, lua_CFunction function);
  virtual bool load();

  template<typename T>
  LuaRef createTable(const T& table,const std::string& name = "DYNAMIC_TABLE"){
    return lua::create_table(this,table);
  }
  
  template<typename T>
  void createGlobal(const std::string& id, const T& data){
    push_struct(m_lua,data);
    lua_setglobal(m_lua,id.c_str());
  }

  template<typename T>
  void readGlobal(const std::string& id, T& out){
    lua::read_global(m_lua,id,out); 
  }

  inline LuaRef operator[](const std::string& name){return root()[name];}
  inline LuaRef operator[](const uint32_t id){return m_allLuaRefs[id];}
  inline LuaRef root(){return m_allLuaRefs[0];}
  inline lua_State* luaState() const{ return m_lua;}
  inline Source source() const {return m_source;}

  void addRef(const std::string& name,LuaRef& ref,uint32_t parentIndex = -1);

  virtual ~Script(){
    ///    lua_close(m_lua);
  }
private:
    
protected:
  void handleTable(const std::string& name,uint32_t parentIndex = -1);
protected:
  lua_State*              m_lua  = nullptr;
  core::file::File        m_file;
  const uint8_t* const    m_buffer;
  const uint32_t          m_bufferSize;
  const Source            m_source;

  std::vector<LuaRef>                m_allLuaRefs;
  std::vector<std::vector<uint32_t>> m_refChildren;
  std::vector<std::string>           m_refNames;
};

template<typename ...T>
bool LuaRef::call(const T&... args) const{
     if(type != FUNCTION){
        assert(false);
        return false;
    }
    lua_rawgeti(owner->luaState(), LUA_REGISTRYINDEX, luaRef);
    //lua_rawgeti(lua, LUA_REGISTRYINDEX, luaClassRef);
    ( push_struct(owner->luaState(),args) , ...);
    lua_pcall(owner->luaState(),sizeof...(T),0,0);      
    return true;      
}; 

template<typename T>
bool LuaRef::read(T& out) const{
    if(null()){
        assert(false);
        return false;
    }
    if(type == FUNCTION){
        assert(false);
        return false;
    }
    if(type == lua::LuaRef::NUMBER){
        LuaRef parent = owner->m_allLuaRefs[parentId];
        parent.load();
        lua_pushnil(owner->luaState());
        while(lua_next(owner->luaState(),-2)){
            std::string name = lua_tostring(owner->luaState(),-2);
            if(name == owner->m_refNames[id]){
                read_struct(owner->luaState(),out);
                lua_pop(owner->luaState(),3);
                return true;
            }
            lua_pop(owner->luaState(),1);
        }
        lua_pop(owner->luaState(),1);
        return false;
    }else{
        load();
        read_struct(owner->luaState(),out);
    }
    return true;
};

}; 
#endif
