#ifndef _LUA_API_H_
#define _LUA_API_H_
#include "context.h"
#include "core.h"
#include "types.h"
#include "sdlwindow.h"
#include <chrono>
#include <primitives.h>
#include <map>
#include <tuple>

struct LuaApi_FunctionCall{
  lua::FlatTable params;
  Context*       context;
};


static void getLuaCallInfo(lua_State* state, LuaApi_FunctionCall& out){
  void*    contextPtr = nullptr;
  lua::read_global(state,std::string("_SCRIPT"),contextPtr);
  out.context = (Context*) contextPtr;
  lua::read_struct(state,out.params);
  assert(contextPtr != nullptr);
}

static int LuaApi_test(lua_State* lua){
  cprint_debug("LuaApi") << "LuaApi_test called" << std::endl;
  return 0;
}

static int LuaApi_addComponent(lua_State* lua){
  LuaApi_FunctionCall callInfo;
  getLuaCallInfo(lua,callInfo);
  std::string type = callInfo.params["type"];
  if(type == "rect"){
    
  }else if(type == "cube"){

  }
  return 0;
}

static int LuaApi_createWindow(lua_State* lua){ 
  LuaApi_FunctionCall callInfo;
  getLuaCallInfo(lua,callInfo);
  callInfo.context->createWindow(callInfo.params["name"],callInfo.params["width"],callInfo.params["height"]);
  return 0;
}


static int LuaApi_loadScript(lua_State* lua){ 
  LuaApi_FunctionCall callInfo;
  getLuaCallInfo(lua,callInfo);
  callInfo.context->loadLuaFile(callInfo.params["path"]);
  return 1;
}

static int LuaApi_sleep(lua_State* lua){
  LuaApi_FunctionCall callInfo;
  getLuaCallInfo(lua,callInfo);
  int ms = callInfo.params["ms"];
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  return 0;
}

static int LuaApi_initRenderContext(lua_State* lua){
  LuaApi_FunctionCall callInfo;
  getLuaCallInfo(lua,callInfo);
  int         windowW       = callInfo.params["width"]; 
  int         windowH       = callInfo.params["height"];
  float       renderScale   = callInfo.params["scale"];
  std::string backend       = callInfo.params["backend"];
  callInfo.context->initRendering(windowW,windowH,renderScale,SDLWindowManager::Backend::GL);
  return 0; 
}

static int LuaApi_registerEventHandler(lua_State* lua){ 
  LuaApi_FunctionCall callInfo;
  getLuaCallInfo(lua,callInfo);

  std::string event = callInfo.params["event"];
  lua::FunctionHandle handler = callInfo.params["handler"];
    
  
  callInfo.context->addLuaEventHandler(event,handler);
  //lua_rawgeti(lua, LUA_REGISTRYINDEX, handler);
  //lua_pcall(lua, 0, 0, 0);

  return 0;
}

static int LuaApi_quit(lua_State* lua){
  LuaApi_FunctionCall callInfo;
  getLuaCallInfo(lua,callInfo);
  callInfo.context->stopLooping();
  return 0; 
}

struct Lua_ApiCall{
  const char* name;
  int(*call)(lua_State*);  
};

constexpr Lua_ApiCall LuaApi[] = {
  {"create",&LuaApi_addComponent},
  {"test",&LuaApi_test},
  {"createWindow",&LuaApi_createWindow},
  {"loadScript",&LuaApi_loadScript},
  {"initRender",&LuaApi_initRenderContext},
  {"quit",&LuaApi_quit},
  {"register",&LuaApi_registerEventHandler},
  {"sleep",&LuaApi_sleep}
};
constexpr uint8_t LuaApi_Len = sizeof(LuaApi) / sizeof(Lua_ApiCall);

#endif
