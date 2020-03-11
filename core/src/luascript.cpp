#include "../include/luascript.h"
#include "../include/log.h"

std::ostream& lua::operator << (std::ostream& out, const lua::Script& script){
    for(int i = 0; i < script.m_allLuaRefs.size(); i++){
        out <<  "[" << script.m_allLuaRefs[i].ID() <<  "] " << script.m_refNames[i] << ": " << script.m_allLuaRefs[i] << std::endl;
    }
    return out;
}

std::ostream& lua::operator<< (std::ostream& out, const lua::LuaRef& ref){
    if(ref.null()){
        out << "NULL";
        return out;
    }

    switch (ref.refType()){
    case lua::LuaRef::NUMBER:
        double d;
        ref.read(d);
        out << "value " << d << " ref " << ref.ref();
        break;
    case lua::LuaRef::STRING:
        {
        std::string s;
        ref.read(s);
        out << "value " << s << " ref " << ref.ref();
        }
        break;
    case lua::LuaRef::FUNCTION:
        out << "value " << "function" << " ref " << ref.ref();
        break;
    case lua::LuaRef::TABLE:
        out << "value " << "table" << " ref " << ref.ref();
        break;
    default:
        out << "value " << "UNDEFINED" << " ref " << ref.ref();
        break;
    }
    return out;
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

const lua::LuaRef lua::LuaRef::operator[](const std::string& name) const{
    std::vector<uint32_t>& children = owner->m_refChildren[id];
    for(uint32_t childId : children){
    if(owner->m_refNames[childId] == name){
            return owner->m_allLuaRefs[childId];
        }
    }
    return LuaRef();
}

std::string lua::LuaRef::name() const {
    return owner->m_refNames[id];
}

void lua::LuaRef::load() const{
    lua_rawgeti(owner->luaState(), LUA_REGISTRYINDEX, luaRef);
}

void lua::Script::handleTable(const std::string& name,uint32_t parentIndex){
    //we assume the table we handle is on the top of the stack
    //stackDump(m_lua);
    LuaRef tableRef;
    tableRef.type        = lua::LuaRef::TABLE;
    tableRef.owner       = this;
    tableRef.luaRef      = luaL_ref(m_lua, LUA_REGISTRYINDEX);
    addRef(name,tableRef,parentIndex);
    lua_rawgeti(m_lua, LUA_REGISTRYINDEX, tableRef.luaRef);

    lua_pushnil(m_lua);
    while(lua_next(m_lua,-2)){
        //stackDump(m_lua);

        int          valueType = lua_type(m_lua, -1);
        int          keyType   = lua_type(m_lua, -2);
        std::string  name      = "";
        
        if(keyType == LUA_TSTRING){
          name = lua_tostring(m_lua,-2);
        }else if(keyType == LUA_TNUMBER){
          name = lua_tointeger(m_lua,-1);
        }

        LuaRef child;        
        child.owner       = this;
        switch(valueType){
            case LUA_TFUNCTION:
            child.type   = LuaRef::FUNCTION;
            child.luaRef = luaL_ref(m_lua, LUA_REGISTRYINDEX);
            addRef(name,child,tableRef.id);
            break;

            case LUA_TNUMBER:
            child.type = LuaRef::NUMBER;
            addRef(name,child,tableRef.id);
            lua_pop(m_lua,1);
            break;

            case LUA_TSTRING:   
            child.type = LuaRef::STRING;
            child.luaRef = luaL_ref(m_lua, LUA_REGISTRYINDEX);
            addRef(name,child,tableRef.id);
            break;

            case LUA_TTABLE:
            handleTable(name,tableRef.id);
            lua_pop(m_lua,1);
            break;
        }
        //stackDump(m_lua);
    }
}
void lua::Script::addRef(const std::string& name,LuaRef& ref,uint32_t parentIndex){
    ref.id               = m_allLuaRefs.size();
    ref.parentId         = parentIndex;
    m_allLuaRefs.push_back(ref);
    m_refNames.push_back( name );
    m_refChildren.push_back(std::vector<uint32_t>());
    if(parentIndex != -1){
        m_refChildren[parentIndex].push_back(ref.id);
    }
    //std::cout << "Adding Ref " << name << " " << ref << std::endl;
    //core::log::Console::instance().error() << 
}

bool lua::Script::load(){
  m_lua = luaL_newstate();
  luaL_openlibs(m_lua);    
  char*  buffer;
  size_t len;
  switch(m_source){
    case FILE:
      m_file.load(core::file::IFile::LAZY);
      buffer = (char*) m_file.data();
      len    = m_file.size();
      cprint_debug("Lua") << "Loading Lua from File" << std::endl;
      break;
    case BUFFER:
      buffer = (char*) m_buffer;
      len    = m_bufferSize;
      cprint_debug("Lua") << "Loading Lua from Buffer" << std::endl;;
      break;
  }
 
  cprint_debug("Lua") << "Loading Buffer " <<  (void*)buffer << " length " << len << std::endl;
  if(buffer == nullptr || len ==0 ){
    return false;
  }


  int error = luaL_loadbuffer(m_lua,buffer,len,"Line") || lua_pcall(m_lua,0,1,0);

  if(error == LUA_OK){
    handleTable("ROOT",-1);
    return true;
  }
  //std::cerr << lua_tostring(m_lua, -1) << std::endl;
  cprint_error() << lua_tostring(m_lua,-1) << std::endl;
  
  return false;
}

void lua::Script::registerFunction(const std::string& name,lua_CFunction function){
  //lua_pushcfunction(m_lua, function);
  //lua_setglobal(m_lua, name.c_str());
  lua_register(m_lua,name.c_str(),function);
}
