#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <atomic>
#include <functional>

#include "primitives.h"
#include "core.h"
#include "sdlwindow.h"
#include "types.h"
#include "../../render/include/gl/glcontext.h"


class Context{
public: 
  typedef std::variant<lua::MappedObject<int>>   SupportedComponents;
  typedef std::variant<int,float>                Event;
  typedef std::function<void(const Event&)>      EventCallback;
 
  Context();
  void loopForever();
  void stopLooping();
  
  /**
   * Initializes the context, can be called after loopForever
   * will create a window and a render context
   */
  void init();
  
  /**
   * creates a new Window with name and dimentions
   *
   */
  void createWindow(const std::string& name, uint32_t w, uint32_t h);
  
  /*
   * Loads and executes a file containing a lua script
   * Can be called from any thread
   * @params path
      path to the file
   */
  void loadLua(const std::string& path);
  
  /*
   *Loads and executes a buffer containing lua
   *Can be called from any thread
   *@params buffer
    pointer to buffer containing lua
    @params size
    size of the buffer
   */
  void loadLua(const uint8_t* buffer, uint32_t size); 
  
  /*
   * Creates an object that has a mapped lua state
   */
  template<typename T>
  lua::MappedObject<T> createLuaObject(T& data){
    lua::MappedObject<T> object = lua::MappedObject<T>(m_luaContext,data);
    object.write();
    m_mappedLuaObjects.push_back(object);
    return object;
  }

  /*
   * Adds a callback to different game events 
   */
  void addEventCallback(uint32_t event);

  inline lua::LuaContext&                luaContext()     {return m_luaContext;}
private:
  void onGLContextInit();
  
  SDLWindowManager       m_windowManager;
  lua::LuaContext        m_luaContext;
  render::gl::GLContext* m_glRenderContext;

  std::vector<SupportedComponents> m_mappedLuaObjects;
  core::task_que::TaskQue<std::function<void()>> m_tasks;

  bool m_running = true;
};
#endif
