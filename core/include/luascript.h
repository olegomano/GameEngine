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

#include <glm/glm.hpp>


namespace lua{

void stackDump (lua_State *L);


class Script;
class Node;

template<typename T>
int push_struct(lua_State* lua, const T& data);

template<typename T>
void read_struct(lua_State* lua, T& out);

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

typedef std::variant<int,double,float,std::string> FlatTableItem;
template<typename T>
LuaRef create_table(Script* script, const T& data);


class Script{
public:
    friend  LuaRef;
    friend  std::ostream& operator<<(std::ostream& os, const Script& dt);

    Script(const std::string& path):m_path(path){
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

    LuaRef operator[](const std::string& name){
        return root()[name];
    }

    LuaRef operator[](const uint32_t id){
        return m_allLuaRefs[id];
    }

    LuaRef root(){
        return m_allLuaRefs[0];
    }

    lua_State* luaState() const{
        return m_lua;
    }

    void addRef(const std::string& name,LuaRef& ref,uint32_t parentIndex = -1);

    virtual ~Script(){
    ///    lua_close(m_lua);
    }

protected:
    void handleTable(const std::string& name,uint32_t parentIndex = -1);
protected:
    const std::string     m_path;
    lua_State*            m_lua  = nullptr;
    
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
