#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <atomic>
#include <functional>

#include "renderer.h"
#include "primitives.h"
#include "core.h"
#include "sdlGL.h"
#include "types.h"
#include "../../render/include/gl/glcontext.h"


class Context{
public: 
  typedef std::variant<lua::MappedObject<Drawable>>   SupportedComponents;
  typedef std::variant<int,float>                     Event;
  typedef std::function<void(const Event&)>           EventCallback;
 
  Context();
  void init();
  void loopForever();
  void stopLooping();
 
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
  inline SdlGlContext&                   graphicsContext(){return m_glContext;}
private:
  void onGLContextInit();

  SdlGlContext                     m_glContext;
  lua::LuaContext                  m_luaContext;
  render::gl::GLContext            m_glRenderContext
  
  std::vector<SupportedComponents> m_mappedLuaObjects;
  std::vector<GLWindow*>           m_windows;
  core::task_que::TaskQue<std::function<void()>> m_tasks;

  bool m_running = true;
};
#endif
