#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <atomic>
#include <functional>
#include <tuple>

#include "primitives.h"
#include "core.h"
#include "sdlwindow.h"
#include "types.h"
#include "luaContext.h"
#include "../../render/include/gl/glcontext.h"

class Context {
public: 
  typedef std::variant<int,float>                Event;
  typedef std::function<void(const Event&)>      EventCallback;
  
  /**
   * Lists of lua Event Handlers
   */
  struct LuaHandlers{
    std::vector<lua::FunctionHandle> frameHandlers;
    std::vector<lua::FunctionHandle> entityCreationHandlers;
    std::vector<lua::FunctionHandle> keyboardHandlers;
  };

  Context();
  void loopForever();
  void stopLooping();
  
  /**
   * Initializes the context, can be called after loopForever
   * will create a window and a render context
   */
  void init();


  template<typename T,typename ...Args>
  void runAsync(T&& func,core::Latch& l,Args&&... args){
    l.close();

    m_tasks.push_back([this,l,func,args...]() mutable {
      (this->*func)((args)...);
      l.open();  
    }); 
  }

  void initRendering(uint32_t w, uint32_t h, float renderScale, SDLWindowManager::Backend b);

  /**
   * creates a new Window with name and dimentions
   *
   */
  void createWindow(const std::string& name, uint32_t w, uint32_t h);
  
  /**
   *
   */
  void createPrimitive();

  /*
   * Loads and executes a file containing a lua script
   * Can be called from any thread
   * @params path
      path to the file
   */
  void loadLuaFile(const std::string& path);
  
  /*
   *Loads and executes a buffer containing lua
   *Can be called from any thread
   *@params buffer
    pointer to buffer containing lua
    @params size
    size of the buffer
   */
  void loadLuaBuffer(const std::string& line); 


  void addInitScript(const std::string& path){
    m_initScript.push_back(path);
  }
  
  /*
   * Adds a callback to different game events 
   */
  void addLuaEventHandler(const std::string& event, const lua::FunctionHandle& h);

  inline lua::LuaContext&                luaContext()     {return m_luaContext;}
private:
  void handleEntityCreatedEvent(uint64_t eventId, void* entity);
  void onGLContextInit();
  
  SDLWindowManager        m_windowManager;
  lua::LuaContext         m_luaContext;
  render::IRenderContext* m_glRenderContext;
  
  std::vector<std::string>                           m_initScript;
  std::unordered_map<uint32_t,render::scene::Entity> m_windowCameras;
  
  core::task_que::TaskQue<std::function<void()>>     m_tasks;
  std::vector<std::tuple<lua::LuaVar,render::scene::Entity>> m_luaEntities;
  LuaHandlers                                       m_luaHandlers;

  bool m_isRenderInit = false;

  uint64_t m_frame = 0;
  bool m_running = true;
};

#endif
