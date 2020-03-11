#ifndef _LUA_API_H_
#define _LUA_API_H_
#include "context.h"
#include "core.h"
#include "types.h"
#include <primitives.h>
#include <map>
#include <tuple>


static std::tuple<uint32_t,Context*> readContext(lua_State* state){
  uint32_t scriptID;
  lua::read_global(state,"_ID",scriptID);
  
  
  //cprint_debug("LuaApi") << "read scriptId " << scriptID << std::endl; 

  void* contextVPtr = nullptr;
  lua::read_global(state,"_SCRIPT",contextVPtr);


  //cprint_debug("LuaApi") << "read context ptr" << contextVPtr << std::endl; 

  Context* context = (Context*)contextVPtr;

  return std::make_tuple(scriptID,context);
}

static int LuaApi_test(lua_State* lua){
  cprint_debug("LuaApi") << "LuaApi_test called" << std::endl;
  return 0;
}

static int LuaApi_addComponent(lua_State* lua){
  //cprint_debug("LuaApi") << "addComponent" << std::endl; 
  auto luaContext   = readContext(lua);
  Context* context  = std::get<Context*>(luaContext);
  uint32_t scriptId = std::get<uint32_t>(luaContext);
  
  //cprint_debug("LuaApi") << "Creating Component from script id " << scriptId << std::endl;
  lua::Script* callingScript = context->getScriptById(scriptId);
 // std::string componentName = lua_tostring(lua,-1);
  
  float xPos;
  float yPos;
  float zPos;

  lua::read_struct(lua,xPos);
  lua_pop(lua,1); 
  lua::read_struct(lua,yPos);
  lua_pop(lua,1);
  lua::read_struct(lua,zPos);
  lua_pop(lua,1);  

  Drawable& d = context->renderer().createDrawable(
      context->renderPrims()[render::Primitives::Type::Triangle]);
  d.position.scale(0.05);
  //cprint_debug("LuaApi") << xPos << " " << yPos  << " " << zPos <<std::endl;
  d.position.setPositionX(xPos);
  d.position.setPositionY(yPos);
  d.position.setPositionZ(zPos);
  GameComponent gc(d,callingScript);
  gc.create();
  gc.luaRef().load();
  context->addComponent(gc);
  return 1;
}

static int LuaApi_createWindow(lua_State* lua){
  auto luaContext   = readContext(lua);
  Context* context  = std::get<Context*>(luaContext);
  uint32_t scriptId = std::get<uint32_t>(luaContext);
  
  std::string windowName;
  uint32_t    width;
  uint32_t    height;

  lua::read_struct(lua,height);
  lua_pop(lua,1);
  lua::read_struct(lua,width);
  lua_pop(lua,1);
  lua::read_struct(lua,windowName);
  lua_pop(lua,1);
  cprint_debug("LuaApi") << "Creating Window " << windowName << "(" << width << "," << height <<")" << std::endl;
  
  context->createWindow(windowName,width,height);

}


static int LuaApi_loadScript(lua_State* lua){
  auto luaContext   = readContext(lua);
  Context* context  = std::get<Context*>(luaContext);
  uint32_t scriptId = std::get<uint32_t>(luaContext);
  
  std::string path;
  lua::read_struct(lua,path);

  cprint_debug("LuaApi") << "Loading Script " << path << std::endl;
  context->loadScript(path);
}


struct Lua_ApiCall{
  const char* name;
  int(*call)(lua_State*);  
};

constexpr Lua_ApiCall LuaApi[] = {
  {"create",&LuaApi_addComponent},
  {"Test",&LuaApi_test},
  {"createWindow",&LuaApi_createWindow},
  {"loadScript",&LuaApi_loadScript}
};

#endif
